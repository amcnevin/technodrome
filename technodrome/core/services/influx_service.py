
from technodrome.utils import get_influxdb_client
from core.structs import DataMetric


def get_last_data_by_loc_and_sensor(location, sensor):
    client = get_influxdb_client()
    results = client.query(f"select last(value),* from technodrome where location='{location}' and sensor='{sensor}' group by *")
    return results.raw


def _convert_to_DataMetric(data) -> dict:

    data_dict = dict()

    series = data.get("series")

    for record in series:
        tags = record.get("tags")
        type = tags.get("type")
        keys = record.get("columns")
        values = record.get("values")
        data_points = []
        for row in values:
            data_points.append({ keys[i]: row[i] for i in range(len(keys))}) 

        dm = DataMetric(
            tags = tags,
            data_points = data_points
        )
        
        data_dict[type] = dm


    return data_dict