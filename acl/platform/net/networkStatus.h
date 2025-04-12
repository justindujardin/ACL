//-----------------------------------------------------------------------------
// Application Core Library
// Copyright (c) 2009-2012 DuJardin Consulting, LLC.
// Portions Copyright (c) 2009 GarageGames, Inc.
//-----------------------------------------------------------------------------

#ifndef ACL_PLATFORM_NETWORKSTATUS_H_
#define ACL_PLATFORM_NETWORKSTATUS_H_

namespace ACLib {
namespace Platform {
namespace Network {
enum State {
  Disconnected = 0,
  Connected,
  Uninitialized,

};
/// @ingroup p2network
/// These are the various status codes that may be returned by functions/methods
/// in the @ref p2network module.
enum Status {
  /// Indicates that the operation completed successfully with no errors.
  Status_NoError = 0,
  /// Indicates the socket was disconnected.
  Status_Disconnected,
  /// The address specified was unreachable.
  Status_UnreachableAddress,
  /// The socket is already bound, or port is in use.
  Status_Again,
  /// Indicates that the delegate is empty, thus the socket cannot be bound.
  Status_DelegateInvalid,
  /// Indicates that the platform object is invalid.
  Status_ObjectInvalid,
  /// Indicates the operation failed because it would block on a non-blocking
  /// socket.
  Status_WouldBlock,
  /// Indicates the operation timed out.
  Status_Timeout,
};
} // namespace Network
} // namespace Platform
} // namespace ACLib
#endif