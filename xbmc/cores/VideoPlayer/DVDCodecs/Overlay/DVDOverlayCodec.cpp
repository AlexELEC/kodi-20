/*
 *  Copyright (C) 2012-2018 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */
#include "DVDOverlayCodec.h"

#include "cores/VideoPlayer/Interface/DemuxPacket.h"
#include "cores/VideoPlayer/Interface/TimingConstants.h"


void CDVDOverlayCodec::GetAbsoluteTimes(double& starttime, double& stoptime, DemuxPacket* pkt)
{
  if (!pkt)
    return;

  double duration = 0.0;
  double pts = starttime;

  // we assume pts from packet is better than what
  // decoder gives us, only take duration
  // from decoder if available
  if (stoptime > starttime)
    duration = stoptime - starttime;
  else if (pkt->duration != DVD_NOPTS_VALUE)
    duration = pkt->duration;

  if (pkt->pts != DVD_NOPTS_VALUE)
    pts = pkt->pts;
  else if (pkt->dts != DVD_NOPTS_VALUE)
    pts = pkt->dts;

  starttime = pts;
  if (duration > 0)
    stoptime = pts + duration;
  else
    stoptime = 0;
}

bool CDVDOverlayCodec::GetSubtitlePacketExtraData(DemuxPacket* pPacket,
                                                  SubtitlePacketExtraData& extraData)
{
  if (!pPacket->pSideData)
    return false;

  AVPacketSideData* sideData{reinterpret_cast<AVPacketSideData*>(pPacket->pSideData)};

  for (int i{0}; i < pPacket->iSideDataElems; i++)
  {
    AVPacketSideData* sd = &sideData[i];

    if (sd->type != AV_PKT_DATA_NEW_EXTRADATA)
      continue;

    if (sd->data && sd->size > 0)
    {
      extraData = *reinterpret_cast<SubtitlePacketExtraData*>(sd->data);
      return true;
    }
  }

  return false;
}
