try: 
	from swiftclient.client import Connection
except Exception as e:	
	print ("import swiftclient's Connection failed")
	import sys
	print(sys.executable)
	print (e)
import os
default_authurl = os.environ['OS_AUTH_URL'] 
default_auth_version = os.environ['OS_AUTH_VERSION']
default_user = os.environ['LOGNAME']
default_key = os.environ['OS_PASSWORD']
default_os_options = {
    'user_domain_name': os.environ['OS_USER_DOMAIN_NAME'],
    'project_domain_name': os.environ['OS_PROJECT_DOMAIN_NAME'],
    'project_name': os.environ['OS_PROJECT_NAME']
}
def swift_connect(_authurl=None, _auth_version=None, _user=None, _key=None):
	if(_authurl==None):
		_authurl=default_authurl
	if(_auth_version==None):
		_auth_version=default_auth_version
	if(_user==None):
		_user=default_user
	if(_key==None):
		_key=default_key
	conn = Connection(
		authurl=_authurl,
		user=_user,
		key=_key,
		auth_version=_auth_version,
		os_options=default_os_options
	)
	return conn
#container = 'new-container'
#conn.put_container(container)
#resp_headers, containers = conn.get_account()
#if container in containers:
#    print("The container was created")

