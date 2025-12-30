#include "watcher.hpp"

#include <QTimer>
#include <QFileInfo>
#include <QFileSystemWatcher>

#include <QDebug> // TODO: Remove me!

#include <chrono>
#include <cassert>

Watcher::Watcher(QObject* parent) noexcept : QObject{parent}, watcher{new QFileSystemWatcher(this)}
{
    connect(watcher, &QFileSystemWatcher::fileChanged, this, [this](const QString& file_path) -> void {
        QFileInfo file_info{file_path};
        if (file_info.exists()) {
            emit fileModified(file_path);
        } else {
            static constexpr unsigned initial_check_count{1};
            recheck_file_with_delay(initial_check_count, std::move(file_info));
        }
    });
    connect(watcher,
            &QFileSystemWatcher::directoryChanged,
            this,
            [this](const QString& directory_path) noexcept -> void { emit directoryModified(directory_path); });
}

QStringList Watcher::get_paths() const noexcept
{
    return paths;
}

void Watcher::set_paths(QStringList new_paths)
{
    if (paths != new_paths) {
        if (!paths.empty()) {
            [[maybe_unused]] const auto not_removed = watcher->removePaths(paths);
            assert(not_removed.empty());
        }
        paths                = std::move(new_paths);
        const auto not_added = watcher->addPaths(paths);
        if (!not_added.empty()) { throw FailedToWatchPathError{not_added[0]}; }
        emit pathsChanged();
    }
}

void Watcher::recheck_file_with_delay(unsigned check_count, QFileInfo file_info)
{
    assert(check_count > 0);
    if (check_count >= max_check_count) { throw FilePathLostError{file_info.filePath()}; }
    static constexpr std::chrono::milliseconds debounce_delay{100};
    QTimer::singleShot(debounce_delay, [this, check_count, file_info = std::move(file_info)]() mutable -> void {
        on_debounce_timer_expired(check_count, std::move(file_info));
    });
}

void Watcher::on_debounce_timer_expired(unsigned check_count, QFileInfo file_info)
{
    const auto file_path = file_info.filePath();
    if (file_info.exists()) {
        qDebug() << "[WATCHER] file" << file_path << "exists after" << check_count << "check";
        const auto file_path = file_info.filePath();
        emit fileModified(file_path);
        if (!watcher->addPath(file_path)) { throw FailedToWatchPathError{file_path}; }
    } else {
        recheck_file_with_delay(++check_count, std::move(file_info));
    }
}

FilePathLostError::FilePathLostError(QStringView path) noexcept : std::runtime_error{format_what_message(path)} {}

std::string FilePathLostError::format_what_message(QStringView path)
{
    return QStringLiteral("file \"%1\" has been lost").arg(path).toStdString();
}

FailedToWatchPathError::FailedToWatchPathError(QStringView path) noexcept
        : std::runtime_error{format_what_message(path)}
{}

std::string FailedToWatchPathError::format_what_message(QStringView path)
{
    return QStringLiteral("failed to watch \"%1\"").arg(path).toStdString();
}
