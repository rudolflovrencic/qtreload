# qtreload

An attempt to make a QML application hot reloadable to increase the
productivity of the development process. The solution must:

- avoid proprietary tools (e.g. [Felgo Hot Reload][1]) and [Qt Creator][2];
- support typical [CMake workflow][2].
- ensure hot reload capability can be turned off and [Ahead-of-Time
  compilation of QML files][11] can be leveraged.

## Build

The project can be configured and built using the typical CMake workflow:

```sh
mkdir build && cmake -B build . -DHOT_RELOAD=true && cmake --build build --parallel
```

Additionally, presets for GCC and Clang are provided. For example, the GCC
preset can be used like this:

```sh
PRESET=gcc; cmake --preset $PRESET -DHOT_RELOAD=true && cmake --build --preset $PRESET
```

## Run and test

1. Run the `./build/qtreload` executable.
2. Edit the `color` property in `qml/First.qml` and you should see the
   changes reflected in the application.

## How it works?

The `HOT_RELOAD` project option controls if hot reload is enabled and
therefore, the ahead-of-time compilation of QML files is disabled.

Files `qml/First.qml` and `qml/Second.qml` are *hot reloadable*, meaning
that when `HOT_RELOAD` is set to `true`, they are not compiled into the
executable. Instead, they are loaded at runtime and reloaded when any changes
occur. This loading is performed by the `HotReloader` component defined in
`HotReloader.qml.in`. `HotReloader` combines [QFileSystemWatcher][9] and
[Loader][10] in order to watch for the file system changes and do the dynamic
reloading of the QML components. The file

Additionally, `QmlEngineCacheManager` singleton provides the ability to clear
the [QQmlEngine][12] component cache which could be preventing the reloading of
changed QML files.

## Quirks

- Many text editors save the file in a few steps so the
  `QFileSystemWatcher::fileChanged` signal might fire a few times for a single
  file save. Consequently, the reported file not exist in the slot function (see
  the note in the [documentation][13]). Therefore, a basic *sleep and recheck*
  method is employed.
- The `Loader` element seems to have its own cache so `HotReloader` adds a time
  stamp to the `Loader` `source` URL in order to ensure a unique URL every time
  and circumvents the cache.
- When changing the value of `HOT_RELOAD`, remove the existing `build`
  directory as the existing files seem to confuse Qt.

## Resources

- Useful Reddit threads: [1][4], [2][5].
- [KDAB YouTube video][6] on the topic and [associated source code][6].
- [qmlpreview documentation][8]

[1]: https://felgo.com/qml-hot-reload
[2]: https://www.qt.io/development/tools/qt-creator-ide
[3]: https://doc.qt.io/qt-6/cmake-build-qml-application.html
[4]: https://www.reddit.com/r/QtFramework/comments/1e5w08n/current_state_of_qml_hot_reload_techniques_or_qml/
[5]: https://www.reddit.com/r/QtFramework/comments/1mwyosq/any_free_qml_hot_reload_tools/
[6]: https://www.youtube.com/watch?v=KSgl59mjBeY
[7]: https://github.com/KDABLabs/kdabtv/tree/master/Qt-Embedded-Cycletime/video2_hot_reload
[8]: https://doc.qt.io/qt-6/qtqml-tooling-qmlpreview.html
[9]: https://doc.qt.io/qt-6/qfilesystemwatcher.html
[10]: https://doc.qt.io/qt-6/qml-qtquick-loader.html
[11]: https://doc.qt.io/qt-6/qtquick-deployment.html#ahead-of-time-compilation
[12]: https://doc.qt.io/qt-6/qqmlengine.html
[13]: https://doc.qt.io/qt-6/qfilesystemwatcher.html#fileChanged
