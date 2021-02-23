from django.db import models
import uuid
from core.models import Device

class DeviceConfig(models.Model):
    config_id = models.UUIDField(db_index=True, unique=True, editable=False, default=uuid.uuid4)
    read_interval = models.BigIntegerField(default=1800000) # 30 minutes
    sleep_interval = models.BigIntegerField(default=300000) # 5 minutes
    sleep = models.BooleanField(default=False)
    publish_topic = models.CharField(null=True, max_length=500)
    heartbeat_topic = models.CharField(null=True, max_length=500)
    callback = models.CharField(null=True, max_length=256)
    device = models.OneToOneField(Device, on_delete=models.CASCADE, related_name="configs")

    class Meta(object):
        app_label = "core"
        db_table = "device_config"