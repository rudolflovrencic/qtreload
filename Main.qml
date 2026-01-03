import lovrencic.qtreload

import QtQuick
import QtQuick.Controls

ApplicationWindow {
    id: root
    visible: true

    Shortcut {
        sequence: ["Ctrl+Shift+R"]
        onActivated: QmlEngineCacheManager.clearCache()
    }

    First {}
}
