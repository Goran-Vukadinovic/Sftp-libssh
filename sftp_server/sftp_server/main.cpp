#include<iostream>
using namespace std;

#include"libssh/libssh.h"
#include"libssh/server.h"
#include"libssh/callbacks.h"
#include"libssh/legacy.h"
#include"libssh/sftp.h"
#include"libssh/ssh2.h"

#define SSHD_USER "libssh"
#define SSHD_PASSWORD "libssh"

#ifndef KEYS_FOLDER
#define KEYS_FOLDER ".\\key\\"
#endif

const int port = 22;

static int authenticate(ssh_session session)
{
	return 0;
}
static int auth_password(const char* user, const char* password)
{
	return 0;
}
void printerror(const char* pText, const char* pErrorInfo)
{
	printf("Error[%s => %s]\n", pText, pErrorInfo);
}

int main(void)
{
	int iError = -1;
	int iAuth = 0;
	ssh_session pSshSession = NULL;
	ssh_bind pSshBind = NULL;
	ssh_message pSshMsg = NULL;
	ssh_channel pSshChannel = NULL;

	iError = ssh_init();

	if (iError == -1)
		printerror("SSH init failed", "Reason Unknown");

	pSshSession = ssh_new(); //Create SSH session

	if (pSshSession == NULL)
		printerror("SSH session creation failed", ssh_get_error(pSshBind));

	pSshBind = ssh_bind_new(); //create a bind object

	if (pSshBind == NULL)
		printerror("SSH session creation failed reason ", ssh_get_error(pSshBind));

	iError = ssh_bind_options_set(pSshBind, SSH_BIND_OPTIONS_BINDPORT, &port);
	iError = ssh_bind_options_set(pSshBind, SSH_BIND_OPTIONS_DSAKEY, "./key/ssh_host_dsa_key");
	//iError = ssh_bind_options_set(pSshBind, SSH_BIND_OPTIONS_RSAKEY, "./key/ssh_host_rsa_key");

	if (iError < 0)
	{
		printerror("Bind options set failed ", ssh_get_error(pSshBind));
	}		

	iError = ssh_bind_listen(pSshBind);

	if (iError < 0)
	{
		printerror("Error listening to socket ", ssh_get_error(pSshBind));
	}		

	cout << "Started sample libssh sshd on port " << port << endl;
	cout << "You can login as the user " << SSHD_USER << " with the password " << SSHD_PASSWORD << endl;

	iError = ssh_bind_accept(pSshBind, pSshSession);
	//ssh_bind_fd_toaccept(pSshBind);
	printf("ssh_bind_accept[%d]\n", iError);
	if (iError == SSH_ERROR)
	{
		printerror("Error accepting a connection ", ssh_get_error(pSshBind));
		return 1;
	}

	if (ssh_handle_key_exchange(pSshSession)) {
		printf("ssh_handle_key_exchange: %s\n", ssh_get_error(pSshSession));
		return 1;
	}

	/* proceed to authentication */
	iAuth = authenticate(pSshSession);
	if (!iAuth)
	{
		printerror("Authentication error: ", ssh_get_error(pSshSession));
		ssh_disconnect(pSshSession);
	}


	ssh_disconnect(pSshSession);
	ssh_bind_free(pSshBind);

	ssh_finalize();
	return 0;
}