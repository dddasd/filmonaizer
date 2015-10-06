#include "plugin.h"

using namespace MediaInfoDLL;

#ifdef Q_OS_WIN
    #define QStrStd QString::fromStdWString
#elif Q_OS_LINUX
    #define QStrStd QString::fromStdString
#else
    #define QStrStd QString::fromStdWString
#endif

void PluginMovieMediaInfo::init_plug() {
    m_Notifyer = 0;
    clear_map();
}

void PluginMovieMediaInfo::clear_map() {
    FmapTags.clear();
    FmapTags.insert("{:duration_hh}","");
    FmapTags.insert("{:duration_mm}","");
    FmapTags.insert("{:duration_ss}","");
    FmapTags.insert("{:duration_ms}","");
    FmapTags.insert("{:codec}","");
    FmapTags.insert("{:height}","");
    FmapTags.insert("{:width}","");
    FmapTags.insert("{:br_video}","");
    FmapTags.insert("{:framerate}","");
    FmapTags.insert("{:audio}","");
    FmapTags.insert("{:format}","");
    //FmapTags.insert("{:subtitles}","");
}

void PluginMovieMediaInfo::result_movie(QString str) {
    if (MediaInfoDLL_Load()==1) {
        if (QFile::exists(str)) {
            MediaInfo mm;
            qDebug() << "MediaInfo - exists";

            if (mm.Open(str.toStdWString())==1) {
                qDebug() << "MediaInfo - open";

                uint dur_ms = QStrStd(mm.Get(Stream_General,0,__T("Duration"))).toUInt();
                FmapTags["{:duration_hh}"] = QString("%1%2").arg(div(div(dur_ms,3600000).quot,10).quot).arg(div(dur_ms,3600000).quot%10);

                dur_ms = div(dur_ms,3600000).rem;
                FmapTags["{:duration_mm}"] = QString("%1%2").arg(div(div(dur_ms,60000).quot,10).quot).arg(div(dur_ms,60000).quot%10);

                dur_ms = div(dur_ms,60000).rem;
                FmapTags["{:duration_ss}"] = QString("%1%2").arg(div(div(dur_ms,1000).quot,10).quot).arg(div(dur_ms,1000).quot%10);

                dur_ms = div(dur_ms,1000).rem;
                FmapTags["{:duration_ms}"] = QString("%1%2%3").arg(div(dur_ms,100).quot).arg(div(dur_ms,10).quot%10).arg(dur_ms%10);

                FmapTags["{:format}"] = QStrStd(mm.Get(Stream_General,0,__T("Format")));

                FmapTags["{:codec}"] = QStrStd(mm.Get(Stream_Video,0,__T("CodecID/Hint")));
                if (FmapTags["{:codec}"].isEmpty())
                {
                    FmapTags["{:codec}"] = QStrStd(mm.Get(Stream_Video,0,__T("Codec/String")));
                    if (FmapTags["{:codec}"].isEmpty())
                        FmapTags["{:codec}"] = QStrStd(mm.Get(Stream_Video,0,__T("Format")));
                }

                FmapTags["{:br_video}"] = QString("%1").arg(int(QStrStd(mm.Get(Stream_Video,0,__T("BitRate"))).toInt()/1000));
                FmapTags["{:width}"] = QStrStd(mm.Get(Stream_Video,0,__T("Width")));
                FmapTags["{:height}"] = QStrStd(mm.Get(Stream_Video,0,__T("Height")));
                FmapTags["{:framerate}"] = QStrStd(mm.Get(Stream_Video,0,__T("FrameRate")));

                qDebug() << "VIDEO_________________________________";
                qDebug() << "CodecID " <<QStrStd(mm.Get(Stream_Video,0,__T("CodecID")));
                qDebug() << "CodecID/Info " <<QStrStd(mm.Get(Stream_Video,0,__T("CodecID/Info")));
                qDebug() << "CodecID/Hint " <<QStrStd(mm.Get(Stream_Video,0,__T("CodecID/Hint")));
                qDebug() << "CodecID/Url " <<QStrStd(mm.Get(Stream_Video,0,__T("CodecID/Url")));
                qDebug() << "CodecID_Description " <<QStrStd(mm.Get(Stream_Video,0,__T("CodecID_Description")));
                qDebug() << "Codec " <<QStrStd(mm.Get(Stream_Video,0,__T("Codec")));
                qDebug() << "Codec/String " <<QStrStd(mm.Get(Stream_Video,0,__T("Codec/String")));
                qDebug() << "Codec/Family " <<QStrStd(mm.Get(Stream_Video,0,__T("Codec/Family")));
                qDebug() << "Codec/Info " <<QStrStd(mm.Get(Stream_Video,0,__T("Codec/Info")));
                qDebug() << "Codec/Url " <<QStrStd(mm.Get(Stream_Video,0,__T("Codec/Url")));
                qDebug() << "Codec/CC " <<QStrStd(mm.Get(Stream_Video,0,__T("Codec/CC")));
                qDebug() << "Codec_Description " <<QStrStd(mm.Get(Stream_Video,0,__T("Codec_Description")));
                qDebug() << "Format " <<QStrStd(mm.Get(Stream_Video,0,__T("Format")));
                qDebug() << "Format/Info " <<QStrStd(mm.Get(Stream_Video,0,__T("Format/Info")));
                qDebug() << "Format_Version " <<QStrStd(mm.Get(Stream_Video,0,__T("Format_Version")));
                qDebug() << "Format/Url " <<QStrStd(mm.Get(Stream_Video,0,__T("Format/Url")));
                qDebug() << "\\VIDEO_________________________________";

                qDebug() << "Stream_General: " << QString::number(mm.Count_Get(Stream_General));
                qDebug() << "Stream_Video: " << QString::number(mm.Count_Get(Stream_Video));
                qDebug() << "Stream_Audio: " << QString::number(mm.Count_Get(Stream_Audio));
                qDebug() << "Stream_Text: " << QString::number(mm.Count_Get(Stream_Text));

                int audio = mm.Count_Get(Stream_Audio);

                for (int i=0;i<audio;i++) {
                    QString codec(QStrStd(mm.Get(Stream_Audio,i,__T("CodecID/Hint"))));
                    if (codec.isEmpty()) {
                        codec = QStrStd(mm.Get(Stream_Audio,i,__T("Codec/String")));
                        if (codec.isEmpty())
                            codec = QStrStd(mm.Get(Stream_Audio,i,__T("Format")));
                    }

                    QString langg(QStrStd(mm.Get(Stream_Audio,i,__T("Language/String"))));
                    if (!langg.isEmpty())
                        langg.append(", ");

                    if (!FmapTags["{:audio}"].isEmpty()) FmapTags["{:audio}"] += "\n";

                    FmapTags["{:audio}"] += langg + codec + ", "+ QString("%1").arg(int(QStrStd(mm.Get(Stream_Audio,i,__T("SamplingRate"))).toInt()/1000)) +
                                            " kHz, " + QString("%1").arg(int(QStrStd(mm.Get(Stream_Audio,i,__T("BitRate"))).toInt()/1000)) + " kbps, "+
                                            QStrStd(mm.Get(Stream_Audio,i,__T("Channel(s)"))) + " channel(s)";

                    qDebug() << "AUDIO_________________________________";
                    qDebug() << "CodecID " <<QStrStd(mm.Get(Stream_Audio,i,__T("CodecID")));
                    qDebug() << "CodecID/Info " <<QStrStd(mm.Get(Stream_Audio,i,__T("CodecID/Info")));
                    qDebug() << "CodecID/Hint " <<QStrStd(mm.Get(Stream_Audio,i,__T("CodecID/Hint")));
                    qDebug() << "CodecID/Url " <<QStrStd(mm.Get(Stream_Audio,i,__T("CodecID/Url")));
                    qDebug() << "CodecID_Description " <<QStrStd(mm.Get(Stream_Audio,i,__T("CodecID_Description")));
                    qDebug() << "Codec " <<QStrStd(mm.Get(Stream_Audio,i,__T("Codec")));
                    qDebug() << "Codec/String " <<QStrStd(mm.Get(Stream_Audio,i,__T("Codec/String")));
                    qDebug() << "Codec/Family " <<QStrStd(mm.Get(Stream_Audio,i,__T("Codec/Family")));
                    qDebug() << "Codec/Info " <<QStrStd(mm.Get(Stream_Audio,i,__T("Codec/Info")));
                    qDebug() << "Codec/Url " <<QStrStd(mm.Get(Stream_Audio,i,__T("Codec/Url")));
                    qDebug() << "Codec/CC " <<QStrStd(mm.Get(Stream_Audio,i,__T("Codec/CC")));
                    qDebug() << "Codec_Description " <<QStrStd(mm.Get(Stream_Audio,i,__T("Codec_Description")));
                    qDebug() << "Format " <<QStrStd(mm.Get(Stream_Audio,i,__T("Format")));
                    qDebug() << "Format/Info " <<QStrStd(mm.Get(Stream_Audio,i,__T("Format/Info")));
                    qDebug() << "Format_Version " <<QStrStd(mm.Get(Stream_Audio,i,__T("Format_Version")));
                    qDebug() << "Format/Url " <<QStrStd(mm.Get(Stream_Audio,i,__T("Format/Url")));
                    qDebug() << "\\AUDIO_________________________________";
                }

                mm.Close();
            }
            MediaInfoDLL_UnLoad();
        }
    }
}

QList<QString> PluginMovieMediaInfo::listTags() {
    QList<QString> ret;

    QMapIterator<QString,QString> i(FmapTags);
    i.toFront();
    while (i.hasNext()) {
        i.next();
        ret.append(i.key());
    }
    return ret;
}

QString PluginMovieMediaInfo::result_tags(QString tt) {
    QMap<QString,QString>::const_iterator i = FmapTags.find(tt);
    if (i.key() == tt) return i.value();
    else return "";
}

QString PluginMovieMediaInfo::getVersion() {
    return version;
}

