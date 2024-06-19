#ifndef GROUNDVIDEORECORDER_H
#define GROUNDVIDEORECORDER_H

#include <QObject>
#include <QProcess>

class FCMavlinkSystem;

class GroundVideoRecorder : public QObject
{
    Q_OBJECT
public:
    explicit GroundVideoRecorder(const FCMavlinkSystem &mavlink_system, QObject *parent = nullptr);

private:
    void armed_changed(bool armed);
    void flight_mode_changed(QString flight_mode);
    QStringList prepare_ffmpeg_pipeline(const QString& video_name);
public:
    void start_video_recording();
    void stop_video_recording();

private:
    QProcess m_record_rocess;
};

#endif // GROUNDVIDEORECORDER_H
