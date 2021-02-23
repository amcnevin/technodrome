from django.contrib import admin
from core.models import Location, Metric, Sensor, Device, DeviceConfig


class MetricInline(admin.TabularInline):
    model = Sensor.metrics.through

class SensorInline(admin.TabularInline):
    model = Device.sensors.through


@admin.register(Metric)
class MetricAdmin(admin.ModelAdmin):
    fields = ('name', 'unit',)
    
    
@admin.register(Sensor)
class SensorAdmin(admin.ModelAdmin):
    fields = ('sensor_name',)
    inlines = [
        MetricInline
    ]
    exclude = ('metrics',)

@admin.register(Device)
class DeviceAdmin(admin.ModelAdmin):
    fields = ('device_name', 'location',)
    inlines = [
        SensorInline
    ]
    exclude = ('sensors',)


@admin.register(Location)
class LocationAdmin(admin.ModelAdmin):
    fields = ('location_name',)

@admin.register(DeviceConfig)
class DeviceConfigAdmin(admin.ModelAdmin):
    list_display = ('device',)
    fieldsets = (
        (None, {
            'fields': ('device',)
        }),
        ('Intervals in millis', {
            'fields': (('read_interval', 'sleep_interval'), 'sleep'),
        }),
        ('MQTT Topics', {
            'fields': (('publish_topic', 'heartbeat_topic'), 'callback'),
        }),
        )