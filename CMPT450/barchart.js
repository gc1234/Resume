// Load and munge data, then make the visualization.
//let fileName = "./cereal-detailed.csv";
let fileName = "cmpt450data3.json";
let featureFields = ["No_Yrs", "EPS_Payout", "TTM_EPS", "PEG", "TTM_ROE",
    "TTM_Growth", "NY_Growth", "DRG_1yr", "Debt", "value"];

d3.json(fileName, function(error, data) {
    let companyMap = {};
    data.forEach(function(d) {
        let company = d.Company;
        companyMap[company] = [];

        // { companyName: [ bar1Val, bar2Val, ... ] }
        featureFields.forEach(function(field) {
            companyMap[company].push( +d[field] );
        });
    });
    makeVis(companyMap);
});

let makeVis = function(companyMap) {
    // Define dimensions of vis
    let margin = { top: 30, right: 50, bottom: 30, left: 50 },
        width  = 550 - margin.left - margin.right,
        height = 250 - margin.top  - margin.bottom;

    // Make x scale
    let xScale = d3.scale.ordinal()
        .domain(featureFields)
        .rangeRoundBands([0, width], 0.1);

    // Make y scale, the domain will be defined on bar update
    let yScale = d3.scale.linear()
        .range([height, 0]);

    // let yScale = d3.scaleLinear()
    //     .range([height, 0]);

    // Create canvas
    let canvas = d3.select("#bar-chart")
        .append("svg")
        .attr("width",  width  + margin.left + margin.right)
        .attr("height", height + margin.top  + margin.bottom)
        .append("g")
        .attr("transform", "translate(" + margin.left + "," + margin.top + ")");

    // Make x-axis and add to canvas
    let xAxis = d3.svg.axis()
        .scale(xScale)
        .orient("bottom");

    canvas.append("g")
        .attr("class", "x axis")
        .attr("transform", "translate(0," + height + ")")
        .call(xAxis);

    // Make y-axis and add to canvas
    let yAxis = d3.svg.axis()
        .scale(yScale)
        .orient("left");

    let yAxisHandleForUpdate = canvas.append("g")
        .attr("class", "y axis")
        .call(yAxis);

    yAxisHandleForUpdate.append("text")
        .attr("transform", "rotate(-90)")
        .attr("y", 6)
        .attr("dy", ".71em")
        .style("text-anchor", "end")
        .text("Value");

    let updateBars = function(data) {
        // First update the y-axis domain to match data
        yScale.domain( d3.extent(data) );
        yAxisHandleForUpdate.call(yAxis);

        let bars = canvas.selectAll(".bar").data(data);

        // Add bars for new data
        bars.enter()
            .append("rect")
            .attr("class", "bar")
            .attr("x", function(d,i) { return xScale( featureFields[i] ); })
            .attr("width", xScale.rangeBand())
            .attr("y", function(d,i) { return yScale(d); })
            .attr("height", function(d,i) { return height - yScale(d); });

        // Update old ones, already have x / width from before
        bars
            .transition().duration(250)
            .attr("y", function(d,i) { return yScale(d); })
            .attr("height", function(d,i) { return height - yScale(d); });


        // Remove old ones
        bars.exit().remove();
    };

    // Handler for dropdown value change
    let dropdownChange = function() {
        let newCompany = d3.select(this).property('value'),
            newData   = companyMap[newCompany];

        updateBars(newData);
    };

    // Get names of companies, for dropdown
    let companies = Object.keys(companyMap).sort();

    let dropdown = d3.select("#bar-chart")
        .insert("select", "svg")
        .on("change", dropdownChange);

    dropdown.selectAll("option")
        .data(companies)
        .enter().append("option")
        .attr("value", function (d) { return d; })
        .text(function (d) {

            return d[0].toUpperCase() + d.slice(1,d.length); // capitalize 1st letter
        });

    let initialData = companyMap[ companies[0] ];
    updateBars(initialData);
};
