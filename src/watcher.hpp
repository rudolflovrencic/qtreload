#ifndef WATCHER_HPP
#define WATCHER_HPP

#include <QObject>
#include <QFileInfo>
#include <QStringList>
#include <QStringView>

#include <QtQmlIntegration/qqmlintegration.h>

#include <string>
#include <stdexcept>
#include <type_traits>

class QFileSystemWatcher;

class Watcher : public QObject {
    Q_OBJECT
    Q_PROPERTY(QStringList paths READ get_paths WRITE set_paths NOTIFY pathsChanged REQUIRED)
    QML_ELEMENT
  private:
    static constexpr unsigned max_check_count{10};
    static_assert(max_check_count > 0);

  private:
    QStringList paths;
    QFileSystemWatcher* watcher;

  public:
    explicit Watcher(QObject* parent = nullptr) noexcept;

    [[nodiscard]] QStringList get_paths() const noexcept;
    void set_paths(QStringList new_paths);

  private:
    void recheck_file_with_delay(unsigned check_count, QFileInfo file_info);
    void on_debounce_timer_expired(unsigned check_count, QFileInfo file_info);

  public:
  signals:
    void pathsChanged();
    void fileModified(QString filePath);
    void directoryModified(QString directoryPath);
};

class FilePathLostError final : public std::runtime_error {
  public:
    explicit FilePathLostError(QStringView path) noexcept;

  private:
    [[nodiscard]] static std::string format_what_message(QStringView path);
};
static_assert(std::is_nothrow_copy_constructible_v<FilePathLostError>);

class FailedToWatchPathError final : public std::runtime_error {
  public:
    explicit FailedToWatchPathError(QStringView path) noexcept;

  private:
    [[nodiscard]] static std::string format_what_message(QStringView path);
};
static_assert(std::is_nothrow_copy_constructible_v<FailedToWatchPathError>);

#endif
