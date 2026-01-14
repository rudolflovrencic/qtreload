#include "watcher.hpp"

#include <QTimer>
#include <QFileInfo>
#include <QFileSystemWatcher>

#include <chrono>
#include <cassert>

namespace lovrencic::qmlreload {

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
}

QStringList Watcher::get_files() const noexcept
{
    return files;
}

void Watcher::set_files(QStringList new_files)
{
    if (files != new_files) {
        for (const auto& file : new_files) {
            const QFileInfo file_info{file};
            if (!file_info.isFile()) { throw PathNotPointingToFileError{file}; }
        }
        if (!files.empty()) {
            [[maybe_unused]] const auto not_removed = watcher->removePaths(files);
            assert(not_removed.empty());
        }
        files                = std::move(new_files);
        const auto not_added = watcher->addPaths(files);
        if (!not_added.empty()) { throw FailedToWatchFileError{not_added[0]}; }
        emit filesChanged();
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
        const auto file_path = file_info.filePath();
        emit fileModified(file_path);
        if (!watcher->addPath(file_path)) { throw FailedToWatchFileError{file_path}; }
    } else {
        recheck_file_with_delay(++check_count, std::move(file_info));
    }
}

FilePathLostError::FilePathLostError(QStringView path) noexcept
        : std::runtime_error{QStringLiteral("file \"%1\" has been lost").arg(path).toStdString()}
{}

FailedToWatchFileError::FailedToWatchFileError(QStringView path) noexcept
        : std::runtime_error{QStringLiteral("failed to watch \"%1\"").arg(path).toStdString()}
{}

PathNotPointingToFileError::PathNotPointingToFileError(QStringView path) noexcept
        : std::runtime_error{QStringLiteral("path \"%1\" not pointing to a file").arg(path).toStdString()}
{}

}
