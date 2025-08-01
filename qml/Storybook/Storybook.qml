import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Storybook

Control {
    id: root

    readonly property alias currentPage: d.currentPage
    readonly property alias darkMode: darkModeCheckBox.checked

    QtObject {
        id: d

        property string currentPage: settings.value("currentPage") ?? "Playground"

        onCurrentPageChanged: {
            testsReRunTimer.restart()
            settings.setValue("currentPage", currentPage)
        }

        function activateInspection(item) {
            inspectionWindow.inspect(item)
            inspectionWindow.show()
            inspectionWindow.requestActivate()
        }

        function performInspection() {
            // Find the items to inspect on the current page
            const getItems = typeName =>
                           InspectionUtils.findItemsByTypeName(
                               viewLoader.item, typeName)
            const items = [
                ...getItems(d.currentPage),
                ...getItems("Custom" + d.currentPage)
            ]

            // Find lowest commont ancestor of found items
            const lca = InspectionUtils.lowestCommonAncestor(
                          items, viewLoader.item)

            // Inspect lca
            if (lca) {
                activateInspection(lca.parent.contentItem === lca
                                   ? lca.parent : lca)
                return
            }

            // Look for the item for inspection on the Overlay, skip items
            // without contentItem which can be, for example, instance of
            // Overlay.modal or Overlay.modeless
            const overlayChildren = root.Overlay.overlay.children

            for (let i = 0; i < overlayChildren.length; i++) {
                const item = overlayChildren[i]

                if (item.contentItem) {
                    activateInspection(item)
                    return
                }
            }

            nothingToInspectDialog.open()
        }
    }

    HotReloader {
        id: reloader

        enabled: hotReloaderControls.enabled

        onReloaded: {
            hotReloaderControls.notifyReload()
            testsReRunTimer.restart()
        }
    }

    TestRunnerController {
        id: testRunnerController
    }

    Timer {
        id: testsReRunTimer

        interval: 100

        onTriggered: {
            if (!settingsLayout.runTestsAutomatically)
                return

            const testFileName = `tst_${d.currentPage}.qml`
            const testsCount = testRunnerController.getTestsCount(testFileName)

            if (testsCount === 0)
                return

            testRunnerController.runTests(testFileName)
        }
    }

    contentItem: SplitView {
        anchors.fill: parent

        ColumnLayout {
            SplitView.preferredWidth: 270

            Pane {
                Layout.fillWidth: true
                Layout.fillHeight: true

                ColumnLayout {
                    width: parent.width
                    height: parent.height

                    Button {
                        Layout.fillWidth: true

                        text: "Settings"

                        onClicked: settingsPopup.open()
                    }

                    CheckBox {
                        id: windowAlwaysOnTopCheckBox

                        Layout.fillWidth: true

                        text: "Always on top"
                        onCheckedChanged: {
                            if (checked)
                                root.ApplicationWindow.window.flags |= Qt.WindowStaysOnTopHint
                            else
                                root.ApplicationWindow.window.flags &= ~Qt.WindowStaysOnTopHint
                        }
                    }

                    CheckBox {
                        id: darkModeCheckBox

                        Layout.fillWidth: true

                        text: "Dark mode"
                    }

                    HotReloaderControls {
                        id: hotReloaderControls

                        Layout.fillWidth: true

                        onForceReloadClicked: reloader.forceReload()
                    }

                    MenuSeparator {
                        Layout.fillWidth: true
                    }

                    FilteredPagesList {
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        currentPage: d.currentPage
                        localPagesPath: StorybookData.localPagesPath
                        model: StorybookData.pagesModel

                        onPageSelected: (page) => d.currentPage = page
                        onStatusClicked: statusStatsDialog.open()
                    }
                }
            }

            Button {
                Layout.fillWidth: true
                text: "Open pages directory"

                onClicked: Qt.openUrlExternally(`file://${StorybookData.localPagesPath}`)
            }
        }

        Page {
            SplitView.fillWidth: true

            Loader {
                id: viewLoader

                anchors.fill: parent
                clip: true

                function remoteLoad(page) {
                    if (viewLoader.sourceComponent) {
                        viewLoader.sourceComponent.destroy()
                        viewLoader.sourceComponent = null
                    }

                    QmlEngineUtils.clearComponentCache()

                    const url = `http://localhost:8080/${d.currentPage}Page.qml`
                    const cmp = Qt.createComponent(url)

                    if (settingsLayout.loadAsynchronously) {
                        viewLoader.sourceComponent = cmp
                    } else {
                        cmp.statusChanged.connect(() => {
                            if (cmp.status === Component.Ready) {
                                viewLoader.sourceComponent = cmp
                            } else if (cmp.status === Component.Error) {
                                viewLoader.sourceComponent = cmp
                            }
                        })
                    }
                }

                Component.onCompleted: {
                    if (StorybookData.mode === StorybookData.Local) {
                        source = Qt.binding(() => reloader.reloading
                                            ? ""
                                            : `file:/${StorybookData.localPagesPath}/${d.currentPage}Page.qml`)
                        asynchronous = Qt.binding(() => !reloader.reloading && settingsLayout.loadAsynchronously)
                    } else {
                        reloader.reloadingChanged.connect(() => {
                            if (reloader.reloading)
                                return

                            remoteLoad(d.currentPage)
                        })
                        d.currentPageChanged.connect(() => remoteLoad(d.currentPage))
                        reloader.reloadingChanged()
                    }
                }

                visible: status === Loader.Ready

                // force reload when `asynchronous` changes
                onAsynchronousChanged: {
                    if (StorybookData.mode === StorybookData.Local) {
                        active = false
                        active = true
                    } else {
                        reloader.reloadingChanged()
                    }
                }
            }

            BusyIndicator {
                anchors.centerIn: parent
                visible: viewLoader.status === Loader.Loading
            }

            Label {
                anchors.centerIn: parent
                visible: viewLoader.status === Loader.Error
                text: "Loading page failed"
            }

            footer: PageToolBar {
                id: pageToolBar

                componentName: d.currentPage
                figmaPagesCount: currentPageModelItem.object
                                 ? currentPageModelItem.object.figma.count : 0

                testRunnerController: testRunnerController

                Instantiator {
                    id: currentPageModelItem

                    model: SingleItemProxyModel {
                        sourceModel: StorybookData.pagesModel
                        roleName: "title"
                        value: d.currentPage
                    }

                    delegate: QtObject {
                        readonly property string title: model.title
                        readonly property var figma: model.figma
                    }
                }

                onFigmaPreviewClicked: {
                    if (!settingsLayout.figmaToken) {
                        noFigmaTokenDialog.open()
                        return
                    }

                    figmaWindow.createObject(root, {
                        figmaModel: currentPageModelItem.object.figma,
                        pageTitle: currentPageModelItem.object.title
                    })
                }

                onInspectClicked: d.performInspection()
            }
        }
    }

    Dialog {
        id: settingsPopup

        anchors.centerIn: Overlay.overlay
        width: 420
        modal: true

        header: Pane {
            background: null

            Label {
                text: "Settings"
            }
        }

        SettingsLayout {
            id: settingsLayout

            width: parent.width
        }
    }

    NoFigmaTokenDialog {
        id: noFigmaTokenDialog
    }

    FigmaLinksCache {
        id: figmaImageLinksCache

        figmaToken: settingsLayout.figmaToken
    }

    InspectionWindow {
        id: inspectionWindow
    }

    NothingToInspectDialog {
        id: nothingToInspectDialog

        pageName: d.currentPage
    }

    StatusStatisticsDialog {
        id: statusStatsDialog

        pagesModel: StorybookData.pagesModel
    }

    Component {
        id: figmaWindow

        FigmaPreviewWindow {
            property string pageTitle
            property alias figmaModel: figmaImagesProxyModel.sourceModel

            title: pageTitle + " - Figma"

            model: FigmaImagesProxyModel {
                id: figmaImagesProxyModel

                figmaLinksCache: figmaImageLinksCache
            }

            onClosing: Qt.callLater(destroy)
        }
    }

    Settings {
        id: settings

        property alias loadAsynchronously: settingsLayout.loadAsynchronously
        property alias runTestsAutomatically: settingsLayout.runTestsAutomatically
        property alias darkMode: darkModeCheckBox.checked
        property alias hotReloading: hotReloaderControls.enabled
        property alias figmaToken: settingsLayout.figmaToken
        property alias windowAlwaysOnTop: windowAlwaysOnTopCheckBox.checked

        property alias x: root.x
        property alias y: root.y
        property alias width: root.width
        property alias height: root.height
    }

    Shortcut {
        sequence: "Ctrl+Shift+I"
        context: Qt.ApplicationShortcut
        onActivated: d.performInspection()
    }

    Shortcut {
        sequences: [StandardKey.Refresh]
        context: Qt.ApplicationShortcut
        onActivated: reloader.forceReload()
    }
}
