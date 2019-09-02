var sliderlogoRedRange = document.getElementById("logoRedRange");
var sliderlogoGreenRange = document.getElementById("logoGreenRange");
var sliderlogoBlueRange = document.getElementById("logoBlueRange");

var frameSliderContainer = document.getElementById("frameSliderContainer");
var logoButton = document.getElementById("logoButton");


var currentURL = document.URL;
currentURL = new URL(currentURL);

function onInputLogo(){
	var r = parseInt(sliderlogoRedRange.value, 10).toString(16);
	var g = parseInt(sliderlogoGreenRange.value, 10).toString(16);
	var b = parseInt(sliderlogoBlueRange.value, 10).toString(16);
	
	if(r.length == 1)
		r = "0" + r;
	if(g.length == 1)
		g = "0" + g;
	if(b.length == 1)
		b = "0" + b;
	
	var color = "#" + r + g + b;
	logoButton.style.background = color;
	logoButton.style.color = invertColor(color);
}

function onLoadFunction() {
	var LR = currentURL.searchParams.get("LR");		// logo red
	var LG = currentURL.searchParams.get("LG");		// logo green
	var LB = currentURL.searchParams.get("LB");		// logo blue
	var LE = currentURL.searchParams.get("LE");		// logo effect
	
	if(LE == null)
		LE = "0";
	
	sliderlogoRedRange.value = parseInt(LR, 10);
	sliderlogoGreenRange.value = parseInt(LG, 10);
	sliderlogoBlueRange.value = parseInt(LB, 10);
	var buttons = document.getElementsByName("logo"); 
	buttons[parseInt(LE, 10)].checked = true;
	
	onInputLogo();
}

function invertColor(hex) {
    if (hex.indexOf('#') === 0) {
        hex = hex.slice(1);
    }
    // convert 3-digit hex to 6-digits.
    if (hex.length === 3) {
        hex = hex[0] + hex[0] + hex[1] + hex[1] + hex[2] + hex[2];
    }
    if (hex.length !== 6) {
        throw new Error('Invalid HEX color.');
    }
    // invert color components
    var r = (255 - parseInt(hex.slice(0, 2), 16)).toString(16),
        g = (255 - parseInt(hex.slice(2, 4), 16)).toString(16),
        b = (255 - parseInt(hex.slice(4, 6), 16)).toString(16);
    // pad each with zeros and return
    return '#' + padZero(r) + padZero(g) + padZero(b);
}

function padZero(str, len) {
    len = len || 2;
    var zeros = new Array(len).join('0');
    return (zeros + str).slice(-len);
}

function logoButtonClick(){
	var nextURL = "/command"
	nextURL += "?LR=" + sliderlogoRedRange.value + "&LG=" + sliderlogoGreenRange.value + "&LB=" + sliderlogoBlueRange.value + "&LE=" + getSelectedButton("logo");
	console.log(nextURL);
	window.location.href = nextURL;
}

function getSelectedButton(name) { 
	var buttons = document.getElementsByName(name); 
	
	for(i = 0; i < buttons.length; i++) { 
		if(buttons[i].checked) 
			return i;
	}
}