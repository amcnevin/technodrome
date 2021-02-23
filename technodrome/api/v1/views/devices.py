from rest_framework.views import APIView
from rest_framework import viewsets
from rest_framework.response import Response
from rest_framework.permissions import AllowAny
from rest_framework.status import HTTP_200_OK
import attr

from core.services import devices as device_service
from api.models import DeviceResponse, DeviceConfigResponse

class DeviceView(viewsets.ViewSet):

    permission_classes=(AllowAny,)

    def get_devices(self, request) -> Response:
        device_list = []
        for device in device_service.get_all_devices():
            device_list.append(self._device_to_response(device=device))

        return Response(device_list, status=HTTP_200_OK)

    def _device_to_response(self, device, metrics=None):
        return attr.asdict(DeviceResponse(
            id= device.device_id,
            name = device.device_name,
            location=device.location.location_name,
            sensors=[sensor.sensor_name for sensor in device.sensors.all()],
            metrics=metrics or {},
        ))
        
    def _device_config_to_response(self, device_config):
        return attr.asdict(DeviceConfigResponse(
            config_id=device_config.config_id,
            read_interval = device_config.read_interval,
            sleep_interval = device_config.sleep_interval,
            sleep = device_config.sleep,
            publish_topic = device_config.publish_topic,
            heartbeat_topic = device_config.heartbeat_topic,
            callback = device_config.callback,
            device = device_config.device.device_id,
            version = hash(device_config)
        ))   

    def get_device(self, request, *args, **kwargs) -> Response:
        device_id = kwargs.get("device_id", None)
        device = device_service.get_device(device_id=device_id)
        metrics = device_service.get_device_latest_metrics(device=device)
        return Response(self._device_to_response(device=device, metrics=metrics), status=HTTP_200_OK)


    def get_device_config(self, request, *args, **kwargs) -> Response:
        device_id = kwargs.get("device_id", None)
        device_config = device_service.get_device_config(device_id=device_id)
        return Response(self._device_config_to_response(device_config), status=HTTP_200_OK)