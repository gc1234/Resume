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
    let xScale = d3.scale.linear()
        .range([0, width]);

    // Make y scale, the domain will be defined on bar update
    let yScale = d3.scale.ordinal()
        .domain(featureFields)
        .rangeRoundBands([height, 0], 0.1);

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
        .orient("top");

    // Make y-axis and add to canvas
    let yAxis = d3.svg.axis()
        .scale(yScale)
        .tickSize(0)
        .orient("left");

    canvas.append("g")
        .attr("class", "y axis")
        //.attr("transform", "translate(" + height + ",0)")
        .call(yAxis);

    let xAxisHandleForUpdate = canvas.append("g")
        .attr("class", "x axis")
        .call(xAxis);

    xAxisHandleForUpdate.append("text")
        .attr("transform", "rotate(-90)")
        .attr("x", 6)
        .attr("dx", ".71em")
        .style("text-anchor", "end");

    let updateBars = function(data) {
        // First update the y-axis domain to match data
        xScale.domain( d3.extent(data) );
        xAxisHandleForUpdate.call(xAxis);

        let bars = canvas.selectAll(".bar").data(data);

        // Add bars for new data
        bars.enter()
            .append("rect")
            .attr("class", "bar")
            .attr("y", function(d,i) { return yScale( featureFields[i] ); })
            .attr("height", yScale.rangeBand())
            .attr("x", function(d,i) { return xScale(d); })
            //.attr("x",0)
            .attr("width", function(d,i) { return width - xScale(d); })
            .attr(transform, rotate);

        // Update old ones, already have x / width from before
        bars
            .transition().duration(250)
            .attr("x", function(d,i) { return xScale(d); })
            .attr("width", function(d,i) { return width - yScale(d); })
            .attr(transform, rotate);

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