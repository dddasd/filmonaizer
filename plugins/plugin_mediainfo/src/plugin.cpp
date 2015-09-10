#include "plugin.h"

using namespace MediaInfoDLL;

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
    FmapTags.insert("{:subtitles}","");
}

void PluginMovieMediaInfo::result_movie(QString str) {
    if (MediaInfoDLL_Load()==1) {
        if (QFile::exists(str)) {
            MediaInfo mm;
            qDebug() << "MediaInfo - exists";
#ifdef Q_OS_WIN
            qDebug() << "MediaInfo - WINDOWS";
            if (mm.Open(str.toStdWString())==1) {
                qDebug() << "MediaInfo - open";

                uint dur_ms = QString::fromStdWString(mm.Get(Stream_General,0,_T("Duration"))).toUInt();
                FmapTags["{:duration_hh}"] = QString("%1%2").arg(div(div(dur_ms,3600000).quot,10).quot).arg(div(dur_ms,3600000).quot%10);

                dur_ms = div(dur_ms,3600000).rem;
                FmapTags["{:duration_mm}"] = QString("%1%2").arg(div(div(dur_ms,60000).quot,10).quot).arg(div(dur_ms,60000).quot%10);

                dur_ms = div(dur_ms,60000).rem;
                FmapTags["{:duration_ss}"] = QString("%1%2").arg(div(div(dur_ms,1000).quot,10).quot).arg(div(dur_ms,1000).quot%10);

                dur_ms = div(dur_ms,1000).rem;
                FmapTags["{:duration_ms}"] = QString("%1%2%3").arg(div(dur_ms,100).quot).arg(div(dur_ms,10).quot%10).arg(dur_ms%10);

                FmapTags["{:format}"] = QString::fromStdWString(mm.Get(Stream_General,0,_T("Format")));

                FmapTags["{:codec}"] = QString::fromStdWString(mm.Get(Stream_Video,0,_T("CodecID/Hint")));
                if (FmapTags["{:codec}"].isEmpty())
                {
                    FmapTags["{:codec}"] = QString::fromStdWString(mm.Get(Stream_Video,0,_T("Codec/String")));
                    if (FmapTags["{:codec}"].isEmpty())
                        FmapTags["{:codec}"] = QString::fromStdWString(mm.Get(Stream_Video,0,_T("Format")));
                }

                FmapTags["{:br_video}"] = QString("%1").arg(int(QString::fromStdWString(mm.Get(Stream_Video,0,_T("BitRate"))).toInt()/1000));
                FmapTags["{:width}"] = QString::fromStdWString(mm.Get(Stream_Video,0,_T("Width")));
                FmapTags["{:height}"] = QString::fromStdWString(mm.Get(Stream_Video,0,_T("Height")));
                FmapTags["{:framerate}"] = QString::fromStdWString(mm.Get(Stream_Video,0,_T("FrameRate")));

                qDebug() << "VIDEO_________________________________";
                qDebug() << "CodecID " <<QString::fromStdWString(mm.Get(Stream_Video,0,_T("CodecID")));
                qDebug() << "CodecID/Info " <<QString::fromStdWString(mm.Get(Stream_Video,0,_T("CodecID/Info")));
                qDebug() << "CodecID/Hint " <<QString::fromStdWString(mm.Get(Stream_Video,0,_T("CodecID/Hint")));
                qDebug() << "CodecID/Url " <<QString::fromStdWString(mm.Get(Stream_Video,0,_T("CodecID/Url")));
                qDebug() << "CodecID_Description " <<QString::fromStdWString(mm.Get(Stream_Video,0,_T("CodecID_Description")));
                qDebug() << "Codec " <<QString::fromStdWString(mm.Get(Stream_Video,0,_T("Codec")));
                qDebug() << "Codec/String " <<QString::fromStdWString(mm.Get(Stream_Video,0,_T("Codec/String")));
                qDebug() << "Codec/Family " <<QString::fromStdWString(mm.Get(Stream_Video,0,_T("Codec/Family")));
                qDebug() << "Codec/Info " <<QString::fromStdWString(mm.Get(Stream_Video,0,_T("Codec/Info")));
                qDebug() << "Codec/Url " <<QString::fromStdWString(mm.Get(Stream_Video,0,_T("Codec/Url")));
                qDebug() << "Codec/CC " <<QString::fromStdWString(mm.Get(Stream_Video,0,_T("Codec/CC")));
                qDebug() << "Codec_Description " <<QString::fromStdWString(mm.Get(Stream_Video,0,_T("Codec_Description")));
                qDebug() << "Format " <<QString::fromStdWString(mm.Get(Stream_Video,0,_T("Format")));
                qDebug() << "Format/Info " <<QString::fromStdWString(mm.Get(Stream_Video,0,_T("Format/Info")));
                qDebug() << "Format_Version " <<QString::fromStdWString(mm.Get(Stream_Video,0,_T("Format_Version")));
                qDebug() << "Format/Url " <<QString::fromStdWString(mm.Get(Stream_Video,0,_T("Format/Url")));
                qDebug() << "\\VIDEO_________________________________";

                qDebug() << "Stream_General: " << QString::number(mm.Count_Get(Stream_General));
                qDebug() << "Stream_Video: " << QString::number(mm.Count_Get(Stream_Video));
                qDebug() << "Stream_Audio: " << QString::number(mm.Count_Get(Stream_Audio));
                qDebug() << "Stream_Text: " << QString::number(mm.Count_Get(Stream_Text));

                int audio = mm.Count_Get(Stream_Audio);

                for (int i=0;i<audio;i++) {
                    QString codec(QString::fromStdWString(mm.Get(Stream_Audio,i,_T("CodecID/Hint"))));
                    if (codec.isEmpty()) {
                        codec = QString::fromStdWString(mm.Get(Stream_Audio,i,_T("Codec/String")));
                        if (codec.isEmpty())
                            codec = QString::fromStdWString(mm.Get(Stream_Audio,i,_T("Format")));
                    }

                    QString langg(QString::fromStdWString(mm.Get(Stream_Audio,i,_T("Language/String"))));
                    if (!langg.isEmpty())
                        langg.append(", ");

                    res_f_f.audio.append(langg + codec + ", "+
                                    QString("%1").arg(int(QString::fromStdWString(mm.Get(Stream_Audio,i,_T("SamplingRate"))).toInt()/1000)) + " kHz, "+
                                    QString("%1").arg(int(QString::fromStdWString(mm.Get(Stream_Audio,i,_T("BitRate"))).toInt()/1000)) + " kbps, "+
                                    QString::fromStdWString(mm.Get(Stream_Audio,i,_T("Channel(s)"))) + " channel(s)");

                    qDebug() << "AUDIO_________________________________";
                    qDebug() << "CodecID " <<QString::fromStdWString(mm.Get(Stream_Audio,i,_T("CodecID")));
                    qDebug() << "CodecID/Info " <<QString::fromStdWString(mm.Get(Stream_Audio,i,_T("CodecID/Info")));
                    qDebug() << "CodecID/Hint " <<QString::fromStdWString(mm.Get(Stream_Audio,i,_T("CodecID/Hint")));
                    qDebug() << "CodecID/Url " <<QString::fromStdWString(mm.Get(Stream_Audio,i,_T("CodecID/Url")));
                    qDebug() << "CodecID_Description " <<QString::fromStdWString(mm.Get(Stream_Audio,i,_T("CodecID_Description")));
                    qDebug() << "Codec " <<QString::fromStdWString(mm.Get(Stream_Audio,i,_T("Codec")));
                    qDebug() << "Codec/String " <<QString::fromStdWString(mm.Get(Stream_Audio,i,_T("Codec/String")));
                    qDebug() << "Codec/Family " <<QString::fromStdWString(mm.Get(Stream_Audio,i,_T("Codec/Family")));
                    qDebug() << "Codec/Info " <<QString::fromStdWString(mm.Get(Stream_Audio,i,_T("Codec/Info")));
                    qDebug() << "Codec/Url " <<QString::fromStdWString(mm.Get(Stream_Audio,i,_T("Codec/Url")));
                    qDebug() << "Codec/CC " <<QString::fromStdWString(mm.Get(Stream_Audio,i,_T("Codec/CC")));
                    qDebug() << "Codec_Description " <<QString::fromStdWString(mm.Get(Stream_Audio,i,_T("Codec_Description")));
                    qDebug() << "Format " <<QString::fromStdWString(mm.Get(Stream_Audio,i,_T("Format")));
                    qDebug() << "Format/Info " <<QString::fromStdWString(mm.Get(Stream_Audio,i,_T("Format/Info")));
                    qDebug() << "Format_Version " <<QString::fromStdWString(mm.Get(Stream_Audio,i,_T("Format_Version")));
                    qDebug() << "Format/Url " <<QString::fromStdWString(mm.Get(Stream_Audio,i,_T("Format/Url")));
                    qDebug() << "\\AUDIO_________________________________";
                }

                mm.Close();
            }
#endif
#ifdef Q_OS_LINUX
            if (mm.Open(QString(film_p.toLocal8Bit()).toStdString())==1) {
                qDebug() << "open";

                uint dur_ms = QString::fromStdString(mm.Get(Stream_General,0,_T("Duration"))).toUInt();
                res_f_f.duration_hh = QString("%1%2").arg(div(div(dur_ms,3600000).quot,10).quot).arg(div(dur_ms,3600000).quot%10);

                dur_ms = div(dur_ms,3600000).rem;
                res_f_f.duration_mm = QString("%1%2").arg(div(div(dur_ms,60000).quot,10).quot).arg(div(dur_ms,60000).quot%10);

                dur_ms = div(dur_ms,60000).rem;
                res_f_f.duration_ss = QString("%1%2").arg(div(div(dur_ms,1000).quot,10).quot).arg(div(dur_ms,1000).quot%10);

                dur_ms = div(dur_ms,1000).rem;
                res_f_f.duration_ms = QString("%1%2%3").arg(div(dur_ms,100).quot).arg(div(dur_ms,10).quot%10).arg(dur_ms%10);

                res_f_f.format = QString::fromStdString(mm.Get(Stream_General,0,_T("Format")));

                res_f_f.codec = QString::fromStdString(mm.Get(Stream_Video,0,_T("CodecID/Hint")));
                if (res_f_f.codec.isEmpty())
                {
                    res_f_f.codec = QString::fromStdString(mm.Get(Stream_Video,0,_T("Codec/String")));
                    if (res_f_f.codec.isEmpty())
                        res_f_f.codec = QString::fromStdString(mm.Get(Stream_Video,0,_T("Format")));
                }
                res_f_f.br_video = QString("%1").arg(int(QString::fromStdString(mm.Get(Stream_Video,0,_T("BitRate"))).toInt()/1000));
                res_f_f.width = QString::fromStdString(mm.Get(Stream_Video,0,_T("Width")));
                res_f_f.height = QString::fromStdString(mm.Get(Stream_Video,0,_T("Height")));
                res_f_f.framerate = QString::fromStdString(mm.Get(Stream_Video,0,_T("FrameRate")));

                qDebug() << "VIDEO_________________________________";
                qDebug() << "CodecID " <<QString::fromStdString(mm.Get(Stream_Video,0,_T("CodecID")));
                qDebug() << "CodecID/Info " <<QString::fromStdString(mm.Get(Stream_Video,0,_T("CodecID/Info")));
                qDebug() << "CodecID/Hint " <<QString::fromStdString(mm.Get(Stream_Video,0,_T("CodecID/Hint")));
                qDebug() << "CodecID/Url " <<QString::fromStdString(mm.Get(Stream_Video,0,_T("CodecID/Url")));
                qDebug() << "CodecID_Description " <<QString::fromStdString(mm.Get(Stream_Video,0,_T("CodecID_Description")));
                qDebug() << "Codec " <<QString::fromStdString(mm.Get(Stream_Video,0,_T("Codec")));
                qDebug() << "Codec/String " <<QString::fromStdString(mm.Get(Stream_Video,0,_T("Codec/String")));
                qDebug() << "Codec/Family " <<QString::fromStdString(mm.Get(Stream_Video,0,_T("Codec/Family")));
                qDebug() << "Codec/Info " <<QString::fromStdString(mm.Get(Stream_Video,0,_T("Codec/Info")));
                qDebug() << "Codec/Url " <<QString::fromStdString(mm.Get(Stream_Video,0,_T("Codec/Url")));
                qDebug() << "Codec/CC " <<QString::fromStdString(mm.Get(Stream_Video,0,_T("Codec/CC")));
                qDebug() << "Codec_Description " <<QString::fromStdString(mm.Get(Stream_Video,0,_T("Codec_Description")));
                qDebug() << "Format " <<QString::fromStdString(mm.Get(Stream_Video,0,_T("Format")));
                qDebug() << "Format/Info " <<QString::fromStdString(mm.Get(Stream_Video,0,_T("Format/Info")));
                qDebug() << "Format_Version " <<QString::fromStdString(mm.Get(Stream_Video,0,_T("Format_Version")));
                qDebug() << "Format/Url " <<QString::fromStdString(mm.Get(Stream_Video,0,_T("Format/Url")));
                qDebug() << "\\VIDEO_________________________________";

                qDebug() << "Stream_General: " << QString::number(mm.Count_Get(Stream_General));
                qDebug() << "Stream_Video: " << QString::number(mm.Count_Get(Stream_Video));
                qDebug() << "Stream_Audio: " << QString::number(mm.Count_Get(Stream_Audio));

                int audio = mm.Count_Get(Stream_Audio);

                for (int i=0;i<audio;i++) {
                    QString codec(QString::fromStdString(mm.Get(Stream_Audio,i,_T("CodecID/Hint"))));
                    if (codec.isEmpty()) {
                        codec = QString::fromStdString(mm.Get(Stream_Audio,i,_T("Codec/String")));
                        if (codec.isEmpty())
                            codec = QString::fromStdString(mm.Get(Stream_Audio,i,_T("Format")));
                    }

                    res_f_f.audio.append(codec + ", "+
                                    QString("%1").arg(int(QString::fromStdString(mm.Get(Stream_Audio,i,_T("BitRate"))).toInt()/1000)) + " kbps, "+
                                    QString::fromStdString(mm.Get(Stream_Audio,i,_T("Channel(s)"))) + " channel(s)");

                    qDebug() << "AUDIO_________________________________";
                    qDebug() << "CodecID " <<QString::fromStdString(mm.Get(Stream_Audio,i,_T("CodecID")));
                    qDebug() << "CodecID/Info " <<QString::fromStdString(mm.Get(Stream_Audio,i,_T("CodecID/Info")));
                    qDebug() << "CodecID/Hint " <<QString::fromStdString(mm.Get(Stream_Audio,i,_T("CodecID/Hint")));
                    qDebug() << "CodecID/Url " <<QString::fromStdString(mm.Get(Stream_Audio,i,_T("CodecID/Url")));
                    qDebug() << "CodecID_Description " <<QString::fromStdString(mm.Get(Stream_Audio,i,_T("CodecID_Description")));
                    qDebug() << "Codec " <<QString::fromStdString(mm.Get(Stream_Audio,i,_T("Codec")));
                    qDebug() << "Codec/String " <<QString::fromStdString(mm.Get(Stream_Audio,i,_T("Codec/String")));
                    qDebug() << "Codec/Family " <<QString::fromStdString(mm.Get(Stream_Audio,i,_T("Codec/Family")));
                    qDebug() << "Codec/Info " <<QString::fromStdString(mm.Get(Stream_Audio,i,_T("Codec/Info")));
                    qDebug() << "Codec/Url " <<QString::fromStdString(mm.Get(Stream_Audio,i,_T("Codec/Url")));
                    qDebug() << "Codec/CC " <<QString::fromStdString(mm.Get(Stream_Audio,i,_T("Codec/CC")));
                    qDebug() << "Codec_Description " <<QString::fromStdString(mm.Get(Stream_Audio,i,_T("Codec_Description")));
                    qDebug() << "Format " <<QString::fromStdString(mm.Get(Stream_Audio,i,_T("Format")));
                    qDebug() << "Format/Info " <<QString::fromStdString(mm.Get(Stream_Audio,i,_T("Format/Info")));
                    qDebug() << "Format_Version " <<QString::fromStdString(mm.Get(Stream_Audio,i,_T("Format_Version")));
                    qDebug() << "Format/Url " <<QString::fromStdString(mm.Get(Stream_Audio,i,_T("Format/Url")));
                    qDebug() << "\\AUDIO_________________________________";
                }

                mm.Close();
            }
#endif
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
