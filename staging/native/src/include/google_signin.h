// Copyright 2018 Google Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef GOOGLE_SIGNIN_GOOGLESIGNIN_H  // NOLINT
#define GOOGLE_SIGNIN_GOOGLESIGNIN_H

#include "future.h"              // NOLINT
#include "google_signin_user.h"  // NOLINT

#if defined(__ANDROID__)
#include <jni.h>
#endif

namespace google {
namespace signin {

class GoogleSignIn {
 public:
  /// <summary>StatusCode</summary>
  /// <remarks> These are based on
  /// https://developers.google.com/android/reference/com/google/android/gms/common/api/CommonStatusCodes
  /// and consistent with what is returned by the Google Sign-In API on both iOS
  /// and Android.
  /// </remarks>
  enum StatusCode {
    /// <summary>The operation was successful, but used the device's cache.
    /// </summary>
    kStatusCodeSuccessCached = -1,

    /// <summary>The operation was successful.</summary>
    kStatusCodeSuccess = 0,

    /// <summary>The result is uninitialized.
    kStatusCodeUninitialized = 100,


    /// <summary> The client attempted to connect to the service with an
    /// invalid account name specified. </summary>
    kStatusCodeInvalidAccount = 5,

    /// <summary>A network error occurred. Retrying should resolve the problem.
    /// </summary>
    kStatusCodeNetworkError = 7,

    /// <summary>An internal error occurred. Retrying should resolve the
    /// problem.</summary>
    kStatusCodeInternalError = 8,

    /// <summary>The application is misconfigured.
    /// This error is not recoverable.</summary>
    /// <remarks>
    /// The developer should look at the logs after this to determine
    /// more actionable information.
    /// </remarks>
    kStatusCodeDeveloperError = 10,

    /// <summary> The operation failed with no more detailed
    /// information.</summary>
    kStatusCodeError = 13,

    /// <summary> A blocking call was interrupted while waiting and did not
    /// run to completion.</summary>
    kStatusCodeInterrupted = 14,

    /// <summary>Timed out while awaiting the result.</summary>
    kStatusCodeTimeout = 15,

    /// <summary>The result was canceled either due to client disconnect
    /// or cancel().</summary>
    kStatusCodeCanceled = 16,

    /// <summary>The client attempted to call a method from an API that
    /// failed to connect.</summary>
    kStatusCodeApiNotConnected = 17,
  };

  // Defines the configuration for the sign-in process.
  struct Configuration {
    /// true to use games signin, false for default signin.
    /// games signing only works on Android.
    bool use_game_signin;
    /// Web client id associated with this app.
    const char *web_client_id;
    /// true for getting an auth code when authenticating.
    /// Note: This may trigger re-consent on iOS.  Ideally, this
    /// is set to true once, and the result sent to the server where the
    /// token is managed forever.
    bool request_auth_code;
    /// true to request to reset the refresh token.  Causes re-consent.
    bool force_token_refresh;
    /// request email address, requires consent.
    bool request_email;
    /// request id token, requires consent.
    bool request_id_token;
    /// used with games signin to show or hide the connecting popup UI.
    /// and to associate an invisible view for other popups.  This is
    /// recommended for VR applications.
    bool hide_ui_popups;
    /// account name to use when authenticating, null indicates use default.
    const char *account_name;
    /// additional scopes to request, requires consent.
    const char **additional_scopes;
    int additional_scope_count;

    Configuration(Configuration const &copy) = default;
    Configuration(Configuration &&move) = delete;
    ~Configuration() = default;

    Configuration &operator=(Configuration const &copy) = delete;
    Configuration &operator=(Configuration &&move) = delete;
  };

  // Holds the result of the sign-in process.
  struct SignInResult {
    GoogleSignInUser *User;
    int StatusCode;

    SignInResult() = default;
    SignInResult(SignInResult const &copy) = default;
    SignInResult(SignInResult &&move) = delete;
    ~SignInResult() = default;

    SignInResult &operator=(SignInResult const &copy) = delete;
    SignInResult &operator=(SignInResult &&move) = delete;
  };

  // Constructs a new instance.
#if defined(__ANDROID__)
  GoogleSignIn(jobject activity, JavaVM *vm);
#else
  GoogleSignIn();
#endif

  // Enables verbose logging.
  void EnableDebugLogging(bool flag);

  // Sets the configuration for the sign-in.  This must be called before
  // calling SignIn().  Calling this invalidates the current sign-in result.
  void Configure(const Configuration &configuration);

  // Starts the authentication process.
  Future<SignInResult> &SignIn();

  // Attempts to sign in silently.
  Future<SignInResult> &SignInSilently();

  // Get the result of the last sign-in.
  const Future<SignInResult> *GetLastSignInResult();

  // Signs out the local user.  Any server side tokens are still valid.
  void SignOut();

  // Disconnects this user from the application.  Invalidates all tokens and
  // consent.
  void Disconnect();

 private:
  class GoogleSignInImpl;
  GoogleSignInImpl *impl_;
};

}  // namespace signin
}  // namespace google

#endif  // GOOGLESIGNIN_GOOGLESIGNIN_H  NOLINT
