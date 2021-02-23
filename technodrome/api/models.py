import attr
import uuid

@attr.s
class DeviceResponse(object):
    id = attr.ib(type=uuid)
    name = attr.ib(type=str)
    location = attr.ib(type=str)
    sensors = attr.ib(type=list, default=[])
    metrics = attr.ib(type=dict, default=None)
    
@attr.s
class DeviceConfigResponse(object):
    config_id = attr.ib(type=uuid)
    read_interval = attr.ib(type=int)
    sleep_interval = attr.ib(type=int)
    sleep = attr.ib(type=bool)
    publish_topic = attr.ib(type=str)
    heartbeat_topic = attr.ib(type=str)
    callback = attr.ib(type=str)
    device = attr.ib(type=uuid)
    version = attr.ib(type=int)
    
