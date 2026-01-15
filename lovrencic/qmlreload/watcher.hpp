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

namespace lovrencic::qmlreload {

class Watcher : public QObject {
    Q_OBJECT
    Q_PROPERTY(QStringList files READ get_files WRITE set_files NOTIFY filesChanged REQUIRED)
    QML_ELEMENT
  private:
    static constexpr unsigned max_check_count{10};
    static_assert(max_check_count > 0);

  private:
    QStringList files;
    QFileSystemWatcher* watcher;

  public:
    explicit Watcher(QObject* parent = nullptr) noexcept;

    [[nodiscard]] QStringList get_files() const noexcept;
    void set_files(QStringList new_files);

  private:
    void recheck_file_with_delay(unsigned check_count, QFileInfo file_info);
    void on_debounce_timer_expired(unsigned check_count, QFileInfo file_info);

  signals:
    void filesChanged();
    void fileModified(QString filePath);
};

class FilePathLostError final : public std::runtime_error {
  public:
    explicit FilePathLostError(QStringView path) noexcept;
};
static_assert(std::is_nothrow_copy_constructible_v<FilePathLostError>);

class FailedToWatchFileError final : public std::runtime_error {
  public:
    explicit FailedToWatchFileError(QStringView path) noexcept;

  private:
    [[nodiscard]] static std::string format_what_message(QStringView path);
};
static_assert(std::is_nothrow_copy_constructible_v<FailedToWatchFileError>);

class PathNotPointingToFileError final : public std::runtime_error {
  public:
    explicit PathNotPointingToFileError(QStringView path) noexcept;
};
static_assert(std::is_nothrow_copy_constructible_v<PathNotPointingToFileError>);

}

#endif
