/*
 *      Copyright (C) 2011 Hendrik Leppkes
 *      http://www.1f0.de
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 */

#pragma once

[uuid("E8E73B6B-4CB3-44A4-BE99-4F7BCB96E491")]
class CLAVCAudio : public CTransformFilter
{
public:
  // constructor method used by class factory
  static CUnknown* WINAPI CreateInstance(LPUNKNOWN pUnk, HRESULT* phr);

  // CTransformFilter
  HRESULT CheckInputType(const CMediaType* mtIn);
  HRESULT CheckTransform(const CMediaType* mtIn, const CMediaType* mtOut);
  HRESULT DecideBufferSize(IMemAllocator * pAllocator, ALLOCATOR_PROPERTIES *pprop);
  HRESULT GetMediaType(int iPosition, CMediaType *pMediaType);

  HRESULT Receive(IMediaSample *pIn);

  // Optional Overrides
  HRESULT CheckConnect(PIN_DIRECTION dir, IPin *pPin);
  HRESULT SetMediaType(PIN_DIRECTION dir, const CMediaType *pmt);

  HRESULT EndOfStream();
  HRESULT BeginFlush();
  HRESULT EndFlush();
  HRESULT NewSegment(REFERENCE_TIME tStart, REFERENCE_TIME tStop, double dRate);

public:
  // Pin Configuration
  const static AMOVIESETUP_MEDIATYPE    sudPinTypesIn[];
  const static int                      sudPinTypesInCount;
  const static AMOVIESETUP_MEDIATYPE    sudPinTypesOut[];
  const static int                      sudPinTypesOutCount;

private:
  CLAVCAudio(LPUNKNOWN pUnk, HRESULT* phr);
  ~CLAVCAudio();

  void ffmpeg_shutdown();

  CMediaType CreateMediaType(AVSampleFormat outputFormat, DWORD nSamplesPerSec, WORD nChannels, DWORD dwChannelMask = 0);
  HRESULT ReconnectOutput(int nSamples, CMediaType& mt);
  HRESULT ProcessBuffer();
  HRESULT Decode(BYTE *p, int buffsize, int &consumed);
  HRESULT GetDeliveryBuffer(IMediaSample **pSample, BYTE **pData);
  HRESULT Deliver(GrowableArray<BYTE> &pBuff, DWORD nSamplesPerSec, WORD nChannels, DWORD dwChannelMask);

private:
  CodecID              m_nCodecId;       // FFMPEG Codec Id
  AVCodec              *m_pAVCodec;      // AVCodec reference
  AVCodecContext       *m_pAVCtx;        // AVCodecContext reference
  AVCodecParserContext *m_pParser;       // AVCodecParserContext reference

  BYTE                 *m_pPCMData;      // Output buffer
  BYTE                 *m_pFFBuffer;     // FFMPEG processing buffer (padded)
	int                  m_nFFBufferSize;  // Size of the FFMPEG processing buffer

  BOOL                 m_fDiscontinuity; // Discontinuity
  REFERENCE_TIME       m_rtStart;        // Start time
  GrowableArray<BYTE>  m_buff;           // Input Buffer
};
