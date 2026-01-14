import lovrencic.qmlreload

import QtQuick

Loader {
    id: root
    required property string entryFile
    required property list<string> watchedFiles
    source: "file://" + entryFile
    anchors.fill: parent

    function initiateReload() {
        active = false;
        source = "";
        cacheManager.clearCache();
    }

    function completeReload() {
        source = "file://" + entryFile;
        active = true;
    }

    QmlEngineCacheManager {
        id: cacheManager
        onCacheCleared: () => root.completeReload()
    }

    Watcher {
        id: watcher
        files: root.watchedFiles
        onFileModified: modifiedFile => root.initiateReload()
    }
}
