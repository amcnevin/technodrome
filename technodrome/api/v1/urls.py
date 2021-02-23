from django.conf.urls import url

from api.v1.views.devices import DeviceView


urlpatterns = [
    url(
        r"^devices/$", 
        DeviceView.as_view({"get": "get_devices"}),
        name="devices",
    ),
    url(
        r"^devices/(?P<device_id>[\w\d\-]+)/$",
        DeviceView.as_view({"get": "get_device"}),
        name="get_device_by_id"        
    ), 
    
    url(
        r"^devices/(?P<device_id>[\w\d\-]+)/config/$",
        DeviceView.as_view({"get": "get_device_config"}),
        name="get_device_config"
    )
]