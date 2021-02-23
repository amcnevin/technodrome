import attr

@attr.s
class DataMetric(object):
    tags = attr.ib(type=dict, default={})
    data_points = attr.ib(type=list, default={})

