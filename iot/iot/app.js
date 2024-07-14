var app = angular.module('myApp', ['ngRoute']);
app.config(['$routeProvider',
function($routeProvider) {
     $routeProvider.
         when('/', {
             templateUrl: 'home.html',
         }).
         when('/soilmoisture', {
             templateUrl: 'soilMoisture.html',
         }).
         when('/carparking', {
             templateUrl: 'parking.html',
         }).
         when('/irrigation', {
             templateUrl: 'irrigation.html',
         }).
         when('/pollution', {
             templateUrl: 'pollution.html',
         }).
         when('/waterlevel', {
             templateUrl: 'waterlevel.html',
         }).
         otherwise({
             redirectTo: '/'
         });
}]);
app.controller('soilMoistureCtrl', function($scope, $http) {
    var sensorchart = Highcharts.chart('soil', {
	    chart: {
	        type: 'spline'
	    },
	    title: {
	        text: 'Time Series Visualization'
	    },
	    xAxis: {
	        type: 'category',
	        title: {
	            text: 'Date Time'
	        }
	    },
	    yAxis: {
	        title: {
	            text: 'Sensor Values'
	        },
	        min: 0
	    },
	    tooltip: {
	        headerFormat: '<b>{series.name}</b><br>',
	        pointFormat: '{point.y:.2f}',
	        style: {
 				color: 'blue',
    			fontWeight: 'bold'
			}
	    },

	    plotOptions: {
	        spline: {
	            marker: {
	                enabled: true
	            }
	        },
	        series: {
 			   pointStart: Date.UTC(2010, 0, 1),
			   pointInterval: 5 * 60 * 1000 // one day
			}
	    },

	    colors: ['#6CF', '#39F', '#06C', '#036', '#000'],

	    series: [{
	        name: "Sensor Data",
	        data: [
	        ]
	    }]
	});
	$scope.getData = function(){
	    $http({
    		method : "POST",
    		url : "retriveSoilSensor.php"
		}).then(function(response) {
	        $scope.data = response.data;
	        console.log($scope.data);
	        sensorchart.update({
	        	series: [{
		        	name: "Soil Moisture",
		        	data: $scope.data
	        	}]
	        });
	    });
	}
	$scope.getData();
	setInterval(function() { 
		$scope.getData();
	}, 1000);
});
app.controller('waterLevelCtrl', function($scope, $http) {
    var waterchart = Highcharts.chart('water', {
	    chart: {
	        type: 'spline'
	    },
	    title: {
	        text: 'Time Series Visualization'
	    },
	    xAxis: {
	        type: 'category',
	        title: {
	            text: 'Date Time'
	        }
	    },
	    yAxis: {
	        title: {
	            text: 'Sensor Values'
	        },
	        min: 0
	    },
	    tooltip: {
	        headerFormat: '<b>{series.name}</b><br>',
	        pointFormat: '{point.y:.2f}',
	        style: {
 				color: 'blue',
    			fontWeight: 'bold'
			}
	    },

	    plotOptions: {
	        spline: {
	            marker: {
	                enabled: true
	            }
	        },
	        series: {
 			   pointStart: Date.UTC(2010, 0, 1),
			   pointInterval: 5 * 60 * 1000 // one day
			}
	    },

	    colors: ['#6CF', '#39F', '#06C', '#036', '#000'],

	    series: [{
	        name: "Water Level",
	        data: [
	        ]
	    }]
	});
	$scope.getData = function(){
	    $http({
    		method : "POST",
    		url : "waterlevel.php"
		}).then(function(response) {
	        $scope.data = response.data;
	        console.log($scope.data);
	        waterchart.update({
	        	series: [{
		        	name: "Water Level",
		        	data: $scope.data
	        	}]
	        });
	    });
	}
	$scope.getData();
	setInterval(function() { 
		$scope.getData();
	}, 1000);
});
app.controller('carParkingCtrl', function($scope, $http) {
    var parkingchart = Highcharts.chart('parking', {
	    chart: {
	        type: 'spline'
	    },
	    title: {
	        text: 'Time Series Visualization'
	    },
	    xAxis: {
	        type: 'category',
	        title: {
	            text: 'Date Time'
	        }
	    },
	    yAxis: {
	        title: {
	            text: 'Parking Availability'
	        },
	        min: 0
	    },
	    tooltip: {
	        headerFormat: '<b>{series.name}</b><br>',
	        pointFormat: '{point.y:.2f}',
	        style: {
 				color: 'blue',
    			fontWeight: 'bold'
			}
	    },

	    plotOptions: {
	        spline: {
	            marker: {
	                enabled: true
	            }
	        },
	        series: {
 			   pointStart: Date.UTC(2010, 0, 1),
			   pointInterval: 5 * 60 * 1000 // one day
			}
	    },

	    colors: ['#6CF', '#39F', '#06C', '#036', '#000'],

	    series: [{
	        name: "Parking Data",
	        data: [
	        ]
	    }]
	});
	$scope.getData = function(){
	    $http({
    		method : "POST",
    		url : "retriveparking.php"
		}).then(function(response) {
	        $scope.data = response.data;
	        console.log($scope.data);
	        parkingchart.update({
	        	series: [{
		        	name: "Parking Availability",
		        	data: $scope.data
	        	}]
	        });
	    });
	}
	$scope.getData();
	setInterval(function() { 
		$scope.getData();
	}, 1000);
});
app.controller('irrigationCtrl', function($scope, $http) {
    var irrigationchart = Highcharts.chart('irrigation', {
	    chart: {
	        type: 'spline'
	    },
	    title: {
	        text: 'Time Series Visualization'
	    },
	    xAxis: {
	        type: 'category',
	        title: {
	            text: 'Date Time'
	        }
	    },
	    yAxis: {
	        title: {
	            text: 'Sensor Values'
	        },
	        min: 0
	    },
	    tooltip: {
	        headerFormat: '<b>{series.name}</b><br>',
	        pointFormat: '{point.y:.2f}',
	        style: {
 				color: 'blue',
    			fontWeight: 'bold'
			}
	    },

	    plotOptions: {
	        spline: {
	            marker: {
	                enabled: true
	            }
	        },
	        series: {
 			   pointStart: Date.UTC(2010, 0, 1),
			   pointInterval: 5 * 60 * 1000 // one day
			}
	    },

	    colors: ['#6CF', '#39F', '#06C', '#036', '#000'],

	    series: [
	    	{
		        name: "Moisture",
		        data: [
		        ]
	    	},
	    	{
		        name: "Humidity",
		        data: [
		        ]
		    },
		    {
		        name: "Temperature",
		        data: [
	     	   ]
	     	}
	    ]	
	});
	$scope.getData = function(){
	    $http({
    		method : "POST",
    		url : "irrigation.php"
		}).then(function(response) {
	        $scope.data = response.data;
	        console.log($scope.data[0]);
	        console.log($scope.data[1]);
	        console.log($scope.data[2]);
	        irrigationchart.update({
	        	series: [
	        	{
		        	name: "Moisture",
		        	data: $scope.data[0]
	        	},
	        	{
		        	name: "Humidity",
		        	data: $scope.data[1]
	        	},
	        	{
		        	name: "Temperature",
		        	data: $scope.data[2]
	        	}
	        	]
	        });
	    });
	}
	$scope.getData();
	setInterval(function() { 
		$scope.getData();
	}, 1000);
});
app.controller('pollutionCtrl', function($scope, $http) {
    var pollutionchart = Highcharts.chart('pollution', {
	    chart: {
	        type: 'spline'
	    },
	    title: {
	        text: 'Time Series Visualization'
	    },
	    xAxis: {
	        type: 'category',
	        title: {
	            text: 'Date Time'
	        }
	    },
	    yAxis: {
	        title: {
	            text: 'Sensor Values'
	        },
	        min: 0
	    },
	    tooltip: {
	        headerFormat: '<b>{series.name}</b><br>',
	        pointFormat: '{point.y:.2f}',
	        style: {
 				color: 'blue',
    			fontWeight: 'bold'
			}
	    },

	    plotOptions: {
	        spline: {
	            marker: {
	                enabled: true
	            }
	        },
	        series: {
 			   pointStart: Date.UTC(2010, 0, 1),
			   pointInterval: 5 * 60 * 1000 // one day
			}
	    },

	    colors: ['#6CF', '#39F', '#06C', '#036', '#000'],

	    series: [
	    	{
		        name: "Air",
		        data: [
		        ]
	    	},
	    	{
		        name: "Sound",
		        data: [
		        ]
		    }
	    ]	
	});
	$scope.getData = function(){
	    $http({
    		method : "POST",
    		url : "retrivepollution.php"
		}).then(function(response) {
	        $scope.data = response.data;
	        console.log($scope.data[0]);
	        console.log($scope.data[1]);
	        pollutionchart.update({
	        	series: [
	        	{
		        	name: "Air",
		        	data: $scope.data[0]
	        	},
	        	{
		        	name: "Sound",
		        	data: $scope.data[1]
	        	}
	        	]
	        });
	    });
	}
	$scope.getData();
	setInterval(function() { 
		$scope.getData();
	}, 1000);
});