from swiftclient.client import Connection

_authurl = 'http://127.0.0.1:8080/auth/v1.0'
_auth_version = '1'
_user = 'test:tester'
_key = 'testing'
_tenant_name = 'test'

conn = Connection(
    authurl=_authurl,
    user=_user,
    key=_key,
    tenant_name=_tenant_name,
    auth_version=_auth_version
)
container = 'new-container'
conn.put_container(container)
#resp_headers, containers = conn.get_account()
#if container in containers:
#    print("The container was created")

