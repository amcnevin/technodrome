from django.db import models
import uuid


class Sensor(models.Model):
    sensor_type_id = models.UUIDField(default=uuid.uuid4)
    sensor_name = models.CharField(max_length=50)

    metrics = models.ManyToManyField("Metric", related_name="sensors")

    class Meta(object):
        app_label = "core"
        db_table = "sensor"

    def __str__(self):
        return f"{self.sensor_name}"