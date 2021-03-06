--- pam_module/pam_oslogin_admin.cc.orig	2018-10-11 16:53:23 UTC
+++ pam_module/pam_oslogin_admin.cc
@@ -14,7 +14,6 @@
 
 #define PAM_SM_ACCOUNT
 #include <security/pam_appl.h>
-#include <security/pam_ext.h>
 #include <security/pam_modules.h>
 #include <sys/stat.h>
 #include <sys/types.h>
@@ -48,7 +47,7 @@ PAM_EXTERN int pam_sm_acct_mgmt(pam_handle_t *pamh, in
   int pam_result = PAM_SUCCESS;
   const char *user_name;
   if ((pam_result = pam_get_user(pamh, &user_name, NULL)) != PAM_SUCCESS) {
-    pam_syslog(pamh, LOG_INFO, "Could not get pam user.");
+    syslog(LOG_INFO, "Could not get pam user.");
     return pam_result;
   }
   string str_user_name(user_name);
@@ -82,7 +81,7 @@ PAM_EXTERN int pam_sm_acct_mgmt(pam_handle_t *pamh, in
   if (HttpGet(url.str(), &response, &http_code) && http_code == 200 &&
       ParseJsonToAuthorizeResponse(response)) {
     if (!file_exists) {
-      pam_syslog(pamh, LOG_INFO,
+      syslog(LOG_INFO,
                  "Granting sudo permissions to organization user %s.",
                  user_name);
       std::ofstream sudoers_file;
