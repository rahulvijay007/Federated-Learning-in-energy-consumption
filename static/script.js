async function fetchData() {
    const response = await fetch('/data');
    const data = await response.json();
    const tableBody = document.getElementById('data-table').getElementsByTagName('tbody')[0];
    tableBody.innerHTML = '';
    data.forEach(row => {
        const newRow = tableBody.insertRow();
        row.forEach(cell => {
            const newCell = newRow.insertCell();
            newCell.textContent = cell;
        });
    });
}

setInterval(fetchData, 5000);
fetchData();
