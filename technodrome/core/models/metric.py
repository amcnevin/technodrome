from django.db import models


class Metric(models.Model):

    name = models.CharField(max_length=50)
    unit = models.CharField(max_length=10)

    class Meta(object):
        app_label = "core"
        db_table = "metric"

    def __str__(self):
        return f"{self.name} - {self.unit}"