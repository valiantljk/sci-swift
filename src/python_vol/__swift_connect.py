try: 
	from swiftclient.client import Connection
except Exception as e:	
	print ("import swiftclient's Connection failed")
	import sys
	print(sys.executable)
	print (e)
default_authurl = 'http://127.0.0.1:8080/auth/v1.0'
default_auth_version = '1'
default_user = 'test:tester'
default_key = 'testing'
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
		auth_version=_auth_version
	)
	return conn
#container = 'new-container'
#conn.put_container(container)
#resp_headers, containers = conn.get_account()
#if container in containers:
#    print("The container was created")

