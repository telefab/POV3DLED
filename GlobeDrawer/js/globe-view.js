/**
 * Display a simplified globe.
 */

/**
 * Number of columns
 */
var GLOBE_WIDTH = 80;

/**
 * Number of LEDs
 */
var GLOBE_HEIGHT = 39;

/**
 * Angle (in degrees) visible from the user point of view
 */
var VISIBLE_ANGLE = 360 * 2. / 5;

/**
 * Minimum width ratio for a line
 */
var MIN_WIDTH_RATIO = 1. / 2;

/**
 * Globe display management
 */
var Globe = Class.extend({
	/**
	 * Constructor
	 */
	init: function(container, width, height, visibleAngle, minWidthRatio) {
		// Save parameters
		this._container = container;
		this._width = width;
		this._height = height;
		this._visibleWidth = Math.floor((1.0 * width * visibleAngle) / 360);
		this._lineReductionRatio = (1. - minWidthRatio) / this._computeLineReduction(0);
		this._visibleXOffset = 0;
		this._cells = [];
		this._image = null;
		this._colors = [];
		this._activeColor = 0;
		this._rotationOrig = null;
		this._activeTouchType = null;

		// Initialize the globe display
		var globeCont = $('<div id="globe" />');
		for (var y = 0; y < this._height; y++) {
			var lineWidth = 100. * (1 - this._computeLineReduction(y) * this._lineReductionRatio);
			var line = $('<div class="globe_line" style="height: ' + (100. / this._height) + '%; width: ' + lineWidth + '%"/>');
			for (var x = 0; x < this._visibleWidth; x++) {
				if (y == 0)
					this._cells[x] = [];
				var cellContainer = $('<div class="globe_cell_container" style="width: ' + (100. / this._visibleWidth) + '%" />');
				var cell = $('<div class="globe_cell" />');
				cell.on('click', this.setCellColor.bind(this, x, y));
				cellContainer.append(cell);
				this._cells[x][y] = cell;
				line.append(cellContainer);
			}
			globeCont.append(line);
		}

		// Initialize the menu
		var menuCont = $('<div id="menu" />');
		var colorCont =  $('<div id="colors" />');
		menuCont.append(colorCont);
		for (var color = 0; color < 8; color++) {
			var colorEl = $('<div class="color" style="background-color: ' + this._computeCSSColor(color) + '" />');
			if (color == this._activeColor)
				colorEl.addClass("selected");
			this._colors[color] = colorEl;
			menuCont.append(colorEl);
			colorEl.click(this.activateColor.bind(this, color));
		}

		// Manage global events
		$("body").on("touchmove", function(event) {
			var touchCount = event.originalEvent.touches.length;
			// Prevent changing the type of action during the same touch (removing/adding fingers)
			if (this._activeTouchType !== null && this._activeTouchType != touchCount)
				return;
			this._activeTouchType = touchCount;
			if (touchCount == 1) {
				// Continuous painting
				var top = event.originalEvent.touches[0].clientY;
				var left = event.originalEvent.touches[0].clientX;
				for (var y = 0; y < this._height; y++) {
					var el = this._cells[0][y];
					var elTop = el.offset().top;
					if (elTop <= top && elTop + el.height() >= top) {
						for (var x = 0; x < this._visibleWidth; x++) {
							el = this._cells[x][y];
							var elLeft = el.offset().left;
							if (elLeft <= left && elLeft + el.width() >= left) {
								this.setCellColor(x, y);
								return;
							}
						}
					}
				}
			} else if (touchCount == 2) {
				// Rotation
				var top = (event.originalEvent.touches[0].clientY + event.originalEvent.touches[1].clientY) / 2;
				var left = (event.originalEvent.touches[0].clientX + event.originalEvent.touches[1].clientX) / 2;
				// Find the current X if any
				var elX = null;
				for (var y = 0; y < this._height; y++) {
					var el = this._cells[0][y];
					var elTop = el.offset().top;
					if (elTop <= top && elTop + el.height() >= top) {
						for (var x = 0; x < this._visibleWidth; x++) {
							el = this._cells[x][y];
							var elLeft = el.offset().left;
							if (elLeft <= left && elLeft + el.width() >= left) {
								elX = x;
								break;
							}
						}
						break;
					}
				}
				if (elX !== null) {
					// Determine current rotation
					if (this._rotationOrig === null)
						this._rotationOrig = elX;
					if (this._rotationOrig != elX) {
						this.rotate(this._rotationOrig - elX);
						this._rotationOrig = elX;
					}
				}
			}
		}.bind(this));
		$("body").on("touchend", function(event) {
			this._rotationOrig = null;
			if (event.originalEvent.touches.length == 0)
				this._activeTouchType = null;
		}.bind(this));
		$("body").on("touchcancel", function(event) {
			this._rotationOrig = null;
			this._activeTouchType = null;
		}.bind(this));

		// Insert elements
		this._container.append(menuCont);
		this._container.append(globeCont);

		// Initialize a black image
		this._initImage();
		// Paint the visible image
		this._refreshImage();
	},

	/**
	 * Select a specific color
	 */
	activateColor: function(activeColor) {
		this._activeColor = activeColor;
		for (var color = 0; color < 8; color++) {
			if (color == activeColor)
				this._colors[color].addClass("selected");
			else
				this._colors[color].removeClass("selected");
		}
	},

	/**
	 * Set a cell color to the active color, update the image
	 */
	setCellColor: function(x, y) {
		var color = this._activeColor;
		var imageX = this._getImageX(x);
		if (this._image[imageX][y] != color) {
			this._image[imageX][y] = color;
			this._refreshCell(x, y, color);
			this._container.trigger("paint", {
				x: x,
				y: y,
				color: color
			});
		}
	},

	/**
	 * Rotate the display of a certain number of columns
	 */
	rotate: function(deltaX) {
		while (deltaX < 0)
			deltaX+= this._width;
		deltaX%= this._width;
		this._visibleXOffset = (this._visibleXOffset + deltaX) % this._width;
		if (deltaX > 0) {
			this._refreshImage();
			this._container.trigger("rotate", {
				offset: this._visibleXOffset,
				delta: deltaX
			});
		}
	},

	/**
	 * Export the current state of the globe
	 */
	exportState: function() {
		return {
			image: this._image,
			visibleXOffset: this._visibleXOffset
		};
	},

	/**
	 * Import a state and set it as current globe state
	 */
	importState: function(state) {
		this._image = state.image;
		this._visibleXOffset = state.visibleXOffset;
		this._refreshImage();
	},

	/**
	 * Get current rotation
	 */
	getVisibleXOffset: function() {
		return this._visibleXOffset;
	},

	/**
	 * Get cells colors
	 */
	getCell: function(x, y) {
		return this._cells[x][y];
	},

	/**
	 * Get the container DOM element
	 */
	getContainer: function() {
		return this._container;
	},

	/**
	 * Get the globe width
	 */
	getWidth: function() {
		return this._width;
	},

	/**
	 * Get the globe height
	 */
	getHeight: function() {
		return this._height;
	},

	/**
	 * Change a cell display to the given color
	 */
	_refreshCell: function(x, y, color) {
		this._cells[x][y].css("backgroundColor", this._computeCSSColor(color));
	},

	/**
	 * Set the visible image from image data
	 */
	_refreshImage: function() {
		for (var x = 0; x < this._visibleWidth; x++) {
			for (var y = 0; y < this._height; y++) {
				this._refreshCell(x, y, this._image[this._getImageX(x)][y]);
			}
		}
	},

	/**
	 * Get the corresponding x on the image
	 * from a painted cell x.
	 */
	_getImageX: function(x) {
		return (x + this._visibleXOffset) % this._width;
	},

	/**
	 * Initialize a black image
	 */
	_initImage: function() {
		this._image = [];
		for (var x = 0; x < this._width; x++) {
			this._image[x] = [];
			for (var y = 0; y < this._height; y++) {
				this._image[x][y] = 0;
			}
		}
	},

	/**
	 * Computes the reduction of line width, from 0 to a fixed value, depending on height
	 */
	_computeLineReduction: function(y) {
		var altitude = y - (this._height - 1.) / 2;
		// Altitude never reached but seen as top the the globe
		var topAlt = (this._height - 1.) / 2 + 1;
		var angle = Math.asin(altitude / topAlt);
		return (1 - Math.cos(angle));
	},

	/**
	 * Returns the CSS color code for the color code
	 */
	_computeCSSColor: function(color) {
		var red = (color & 1) == 0 ? 0 : 255;
		var green = (color & 2) == 0 ? 0 : 255;
		var blue = (color & 4) == 0 ? 0 : 255;
		return "rgb(" + red + "," + green + "," + blue + ")";
	}
});

/**
 * Image storage in persistant memory
 */
var DrawingStorage = Class.extend({
	/**
	 * Constructor: key name for stored drawings
	 */
	 init: function(key) {
	 	this._key = key;
	 	this._data = null;
	 	this._loadData();
	 },

	 /**
	  * Load data from local storage
	  */
	 _loadData: function() {
	 	var str = localStorage.getItem(this._key);
	 	if (str === null)
	 		this._data = [];
	 	else
	 		this._data = JSON.parse(str);
	 },

	 /**
	  * Save data to local storage
	  */
	 _saveData: function() {
	 	localStorage.setItem(this._key, JSON.stringify(this._data));
	 }
});

/**
 * Data sender to the Arduino
 */
var ArduinoSync = Class.extend({
	/**
	 * Constructor: take the synchronized globe HTML element
	 */
	init: function(globe, address) {
		this._globe = globe;
		this._address = address;
		this._changedPixels = [];
		this._lastVisibleXOffset = 0;
		this._sendAll = false;
		this._globe.getContainer().on("paint", this._onPaint.bind(this));
		this._sendData();
	},

	/**
	 * Get the address
	 */
	getAddress: function() {
		return this._address;
	},

	/**
	 * Set the address
	 */
	setAddress: function(address) {
		this._address = address;
	},

	/**
	 * React to a globe paint event.
	 */
	_onPaint: function(event, data) {
		if (this._changedPixels.length >= this._globe.getWidth() * this._globe.getHeight() / 3) {
			this._sendAll = true;
			return;
		}
		for (var i = 0; i < this._changedPixels.length; i++) {
			var pixel = this._changedPixels[i];
			if (pixel.x == data.x && pixel.y == data.y) {
				pixel.color = data.color;
				return;
			}
		}
		this._changedPixels.push(data);
	},

	/**
	 * Send data to the globe
	 */
	_sendData: function() {
		if (this._sendAll) {
			// Send all data from the globe
			var data = this._convertNumber(3, 0); // 0 means all data is sent
			data+= this._convertNumber(3, this._globe._lastVisibleXOffset);
			for (var x = 0; x < this._globe.getWidth(); x++) {
				for (var y = 0; y < this._globe.getHeight(); y++) {
					data+= this._convertNumber(3, this._globe.getCell(x, y));
				}
			}
			$.post("http://" + this._address + "/sendData", data);
		} else if (this._changedPixels.length > 0 || this._lastVisibleXOffset != this._globe.getVisibleXOffset()) {
			// Send incremental data
			var data = this._convertNumber(3, 1); // 1 means a change is sent
			data+= this._convertNumber(3, this._globe.getVisibleXOffset());
			for (var i = 0; i < this._changedPixels.length; i++) {
				var pixel = this._changedPixels[i];
				data+= this._convertNumber(3, pixel.x);
				data+= this._convertNumber(3, pixel.y);
				data+= this._convertNumber(3, pixel.color);
			}
			$.post("http://" + this._address + "/sendData", data);
		}
		this._changedPixels = [];
		this._lastVisibleXOffset = this._globe.getVisibleXOffset();
		this._sendAll = false;
		setTimeout(this._sendData.bind(this), 500);
	},

	/**
	 * Convert a number to string
	 */
	_convertNumber: function(width, value) {
		var str = '' + value;
		while (str.length < width)
			str = '0' + str;
		return str;
	}

});

$(function() {
	var storage = new DrawingStorage("drawings");
	var globeView = new Globe($("body"), GLOBE_WIDTH, GLOBE_HEIGHT, VISIBLE_ANGLE, MIN_WIDTH_RATIO);
	var arduinoSync = new ArduinoSync(globeView, "192.168.0.104");
	$("#menu").append('<a href="#" id="config">+</a>');
	$("#config").click(function(){
		var response = prompt("Adresse du globe : ", arduinoSync.getAddress());
		if (response)
			arduinoSync.setAddress(response);
	});
});