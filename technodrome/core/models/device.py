from django.db import models
import uuid


class Device(models.Model):

    device_id = models.UUIDField(db_index=True, unique=True, editable=False, default=uuid.uuid4)
    device_name = models.CharField(max_length=50, default=None)
    location = models.ForeignKey("Location", on_delete=models.CASCADE)
    
    sensors = models.ManyToManyField("Sensor", related_name="devices")

    class Meta(object):
        app_label = "core"
        db_table = "device"


    def __str__(self):
        return f"{self.device_id}: {self.device_name} : {self.location}"
