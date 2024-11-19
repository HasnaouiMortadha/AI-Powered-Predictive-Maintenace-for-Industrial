function fetchSensorData(endpoint, elementId) {
    fetch(endpoint)
        .then(response => response.json())
        .then(data => {
            document.getElementById(elementId).innerText = JSON.stringify(data, null, 2);
        })
        .catch(error => {
            document.getElementById(elementId).innerText = "Error fetching data.";
            console.error("Error:", error);
        });
}

function updateData() {
    fetchSensorData("/adxl345", "adxl345-data");
    fetchSensorData("/dht22", "dht22-data");
    fetchSensorData("/mpx5700ap", "mpx5700ap-data");
    fetchSensorData("/ina219", "ina219-data");
    fetchSensorData("/ky038", "ky038-data");
}

// Update data every 5 seconds
setInterval(updateData, 5000);
updateData();
