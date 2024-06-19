#include "ground_video_recorder.h"

#include <QDebug>
#include <QDateTime>

#include "telemetry/models/fcmavlinksystem.h"
#include <logging/hudlogmessagesmodel.h>
#include <logging/logmessagesmodel.h>

GroundVideoRecorder::GroundVideoRecorder(const FCMavlinkSystem &mavlink_system, QObject *parent)
    : QObject{parent}
{
    HUDLogMessagesModel::instance().add_message_info("Init GroundVideoRecorder");
    connect(&mavlink_system, &FCMavlinkSystem::armed_changed, this, &GroundVideoRecorder::armed_changed);
    connect(&mavlink_system, &FCMavlinkSystem::flight_mode_changed, this, &GroundVideoRecorder::flight_mode_changed);
}

QStringList GroundVideoRecorder::prepare_ffmpeg_pipeline(const QString& video_filename){
    QStringList args;

    args << "-f"            << "x11grab"
         << "-video_size"   << "1920x1200"
         << "-framerate"    << "30"
         << "-i"            << ":0.0+0,0"
         << "-vcodec"       << "libx264"
         << "-vf"           << "format=yuv420p"
         << video_filename;

    return args;
}

void GroundVideoRecorder::start_video_recording() {
    auto current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd-hh:mm:ss");
    auto video_filename = "video_" + current_date_time + ".mp4";

    const auto ffmpeg_pipeline = prepare_ffmpeg_pipeline(video_filename);

    qDebug() << "ffmpeg_pipeline " << ffmpeg_pipeline;

    m_record_rocess.start("ffmpeg", ffmpeg_pipeline);
}

void GroundVideoRecorder::stop_video_recording() {
    m_record_rocess.write("q\n\r");
}

void GroundVideoRecorder::armed_changed(bool armed)
{
    if (armed)
    {
        qDebug() << "Starting ground video recording";
        HUDLogMessagesModel::instance().add_message_info("Starting ground video recording");
        start_video_recording();
    }
    else
    {
        qDebug() << "Stopping ground video recording";
        HUDLogMessagesModel::instance().add_message_info("Stopping ground video recording");
        stop_video_recording();
    }
}

void GroundVideoRecorder::flight_mode_changed(QString flight_mode) {
    qDebug() << "Flight mode changed";
    start_video_recording();
    HUDLogMessagesModel::instance().add_message_info("Flight mode changed");
    LogMessagesModel::instanceGround().add_message_warn("QOpenHD", "Flight mode changed");
}