/* <!-- copyright */
/*
 * aria2 - The high speed download utility
 *
 * Copyright (C) 2006 Tatsuhiro Tsujikawa
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * In addition, as a special exception, the copyright holders give
 * permission to link the code of portions of this program with the
 * OpenSSL library under certain conditions as described in each
 * individual source file, and distribute linked combinations
 * including the two.
 * You must obey the GNU General Public License in all respects
 * for all of the code used other than OpenSSL.  If you modify
 * file(s) with this exception, you may extend this exception to your
 * version of the file(s), but you are not obligated to do so.  If you
 * do not wish to do so, delete this exception statement from your
 * version.  If you delete this exception statement from all source
 * files in the program, then also delete it here.
 */
/* copyright --> */
#ifndef D_DEFAULT_BT_MESSAGE_DISPATCHER_H
#define D_DEFAULT_BT_MESSAGE_DISPATCHER_H

#include "BtMessageDispatcher.h"

#include <deque>

#include "a2time.h"
#include "Command.h"

namespace aria2 {

class DownloadContext;
class PeerStorage;
class PieceStorage;
class BtMessage;
class BtMessageFactory;
class Peer;
class Piece;
class RequestGroupMan;
class PeerConnection;

class DefaultBtMessageDispatcher : public BtMessageDispatcher {
private:
  cuid_t cuid_;
  std::deque<std::unique_ptr<BtMessage> > messageQueue_;
  std::deque<std::unique_ptr<RequestSlot>> requestSlots_;
  DownloadContext* downloadContext_;
  PeerStorage* peerStorage_;
  PieceStorage* pieceStorage_;
  PeerConnection* peerConnection_;
  BtMessageFactory* messageFactory_;
  std::shared_ptr<Peer> peer_;
  RequestGroupMan* requestGroupMan_;
  time_t requestTimeout_;
public:
  DefaultBtMessageDispatcher();

  virtual ~DefaultBtMessageDispatcher();

  virtual void addMessageToQueue(std::unique_ptr<BtMessage> btMessage);

  virtual void sendMessages();

  // For unit tests without PeerConnection
  void sendMessagesInternal();

  virtual void doCancelSendingPieceAction
  (size_t index, int32_t begin, int32_t length);

  virtual void doCancelSendingPieceAction(const std::shared_ptr<Piece>& piece);

  virtual void doAbortOutstandingRequestAction(const std::shared_ptr<Piece>& piece);

  virtual void doChokedAction();

  virtual void doChokingAction();

  virtual void checkRequestSlotAndDoNecessaryThing();

  virtual bool isSendingInProgress();

  virtual size_t countMessageInQueue() {
    return messageQueue_.size();
  }

  virtual size_t countOutstandingRequest()
  {
    return requestSlots_.size();
  }

  virtual bool isOutstandingRequest(size_t index, size_t blockIndex);

  virtual const RequestSlot* getOutstandingRequest
  (size_t index, int32_t begin, int32_t length);

  virtual void removeOutstandingRequest(const RequestSlot* slot);

  virtual void addOutstandingRequest(std::unique_ptr<RequestSlot> requestSlot);

  virtual size_t countOutstandingUpload();

  const std::deque<std::unique_ptr<BtMessage>>& getMessageQueue() const
  {
    return messageQueue_;
  }

  const std::deque<std::unique_ptr<RequestSlot>>& getRequestSlots() const
  {
    return requestSlots_;
  }

  void setPeer(const std::shared_ptr<Peer>& peer);

  void setDownloadContext(DownloadContext* downloadContext);

  void setPieceStorage(PieceStorage* pieceStorage);

  void setPeerStorage(PeerStorage* peerStorage);

  void setBtMessageFactory(BtMessageFactory* factory);

  void setRequestGroupMan(RequestGroupMan* rgman);

  void setCuid(cuid_t cuid)
  {
    cuid_ = cuid;
  }

  void setRequestTimeout(time_t requestTimeout)
  {
    requestTimeout_ = requestTimeout;
  }

  void setPeerConnection(PeerConnection* peerConnection)
  {
    peerConnection_ = peerConnection;
  }
};

} // namespace aria2

#endif // D_DEFAULT_BT_MESSAGE_DISPATCHER_H
