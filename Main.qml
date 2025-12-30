import lovrencic.qtreload

import QtQuick
import QtQuick.Controls

ApplicationWindow {
    id: root
    visible: true

    HotReloader {
        id: hotReloader
        anchors.fill: parent
    }
}
