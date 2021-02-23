from django.db import models
import uuid

class Location(models.Model):

    location_id = models.UUIDField(db_index=True, unique=True, editable=False, default=uuid.uuid4)
    location_name = models.CharField(max_length=50)

    class Meta(object):
        app_label = "core"
        db_table = "location"

    def __str__(self):
        return f"{self.location_name}"
