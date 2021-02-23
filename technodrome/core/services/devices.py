from . import influx_service
from core.models import Device, DeviceConfig


def get_all_devices():
    """
    get all devices
    """
    return Device.objects.all()

def get_device(device_id):
    """
    get device by it's id
    """
    return Device.objects.get(device_id=device_id)

def get_device_latest_metrics(device: Device):
    """
    get the last data points for a device and its first sensor
    TODO scale this once we have multiple sensors on a device
    """
    data = influx_service.get_last_data_by_loc_and_sensor(device.location.location_name, device.sensors.first())
    return influx_service._convert_to_DataMetric(data)

def get_device_config(device_id):
    return DeviceConfig.objects.get(device__device_id=device_id)