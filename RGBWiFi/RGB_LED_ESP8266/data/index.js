const URL_INDEX_OF_EFFECT             	= 1;
const URL_INDEX_OF_SPEED              	= 2;
const URL_INDEX_OF_BRIGHTNESS         	= 3;

const URL_INDEX_OF_R                  	= 5;
const URL_INDEX_OF_G                  	= 6;
const URL_INDEX_OF_B                  	= 7;
const URL_OFFSET_TO_NEXT_STRIP        	= 10;

const LOGO_URL_OFFSET					= 0 * URL_OFFSET_TO_NEXT_STRIP;
const FRAME_URL_OFFSET					= 1 * URL_OFFSET_TO_NEXT_STRIP;


//EXPAND
var sliderLogoR = document.getElementById("rangeLogoR");
var sliderLogoG = document.getElementById("rangeLogoG");
var sliderLogoB = document.getElementById("rangeLogoB");
var sliderLogoBr = document.getElementById("rangeLogoBrightness");
var sliderLogoSp = document.getElementById("rangeLogoSpeed");
var buttonLogo = document.getElementById("buttonLogo");

var sliderFrameR = document.getElementById("rangeFrameR");
var sliderFrameG = document.getElementById("rangeFrameG");
var sliderFrameB = document.getElementById("rangeFrameB");
var sliderFrameBr = document.getElementById("rangeFrameBrightness");
var sliderFrameSp = document.getElementById("rangeFrameSpeed");
var buttonFrame = document.getElementById("buttonFrame");

var currentURL = new URL(document.URL);

function onInput(id){
	//EXPAND
	switch(id) {
	case 'sliderContainerLogo':
		var _sliderR = sliderLogoR;
		var _sliderG = sliderLogoG;
		var _sliderB = sliderLogoB;
		var _button = buttonLogo;
		break;
	case 'sliderContainerFrame':
		var _sliderR = sliderFrameR;
		var _sliderG = sliderFrameG;
		var _sliderB = sliderFrameB;
		var _button = buttonFrame;
		break;
	default:
		throw "DEBUG HTML CODE YOU MORON!!!";
	}
	
	var r = parseInt(_sliderR.value, 10).toString(16);
	var g = parseInt(_sliderG.value, 10).toString(16);
	var b = parseInt(_sliderB.value, 10).toString(16);
	
	if(r.length === 1)
		r = "0" + r;
	if(g.length === 1)
		g = "0" + g;
	if(b.length === 1)
		b = "0" + b;
	
	var color = "#" + r + g + b;
	_button.style.background = color;
	_button.style.color = invertColor(color);
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
	if(r.length === 1)
		r = "0" + r;
	if(g.length === 1)
		g = "0" + g;
	if(b.length === 1)
		b = "0" + b;
	
	return "#" + r + g + b;
}

function onLoadFunction() {
	var valuePairs = location.search.substr(1).split("&");
	
	for(i = 0; i < valuePairs.length; i++) { 
		var valuePair = valuePairs[i];
		switch(Math.floor(valuePair[0] / URL_OFFSET_TO_NEXT_STRIP)){
			case 0:
				break;
			case 1:
				break;
			FELROBBAN AZ AGYAM, FOLYT KÖV
		}
	}
	
	if(LE == null)
		LE = "0";
	
	sliderLogoR.value = parseInt(LR, 10);
	sliderLogoG.value = parseInt(LG, 10);
	sliderLogoB.value = parseInt(LB, 10);
	var buttons = document.getElementsByName("logo"); 
	buttons[parseInt(LE, 10)].checked = true;
	
	//EXPAND
	onInput('sliderContainerLogo');
	onInput('sliderContainerFrame');
}

function buttonClick(id){
	if(id == 'buttonAll'){
		//EXPAND
		window.location.href = "/command?" + getURLOf('buttonLogo') + '&' + getURLOf('buttonFrame');
	}
	
	window.location.href = "/command?" + getURLOf(id);
}

/*
const URL_INDEX_OF_EFFECT             = 1;
const URL_INDEX_OF_SPEED              = 2;
const URL_INDEX_OF_BRIGHTNESS         = 3;

const URL_INDEX_OF_R                  = 5;
const URL_INDEX_OF_G                  = 6;
const URL_INDEX_OF_B                  = 7;
const URL_OFFSET_TO_NEXT_STRIP        = 10;
*/

function getURLOf(id){
	var ret = "";
	switch(id) {
	case 'buttonLogo':
		var _sliderR = sliderLogoR;
		var _sliderG = sliderLogoG;
		var _sliderB = sliderLogoB;
		var _sliderBr = sliderLogoBr;
		var _sliderSp = sliderLogoSp;
		var _button = buttonLogo;
		var name = "logo";
		var offset = 0;
		break;
	case 'buttonFrame':
		var _sliderR = sliderFrameR;
		var _sliderG = sliderFrameG;
		var _sliderB = sliderFrameB;
		var _sliderBr = sliderFrameBr;
		var _sliderSp = sliderFrameSp;
		var _button = buttonFrame;
		var name = "frame";
		var offset = 1 * URL_OFFSET_TO_NEXT_STRIP;
		break;
	default:
		throw "DEBUG HTML CODE YOU MORON!!!";
	}
	ret += (offset + URL_INDEX_OF_EFFECT) + "=" + getSelectedButton(name);
	ret += "&" + (offset + URL_INDEX_OF_SPEED) + "=" + _sliderSp.value;
	ret += "&" + (offset + URL_INDEX_OF_BRIGHTNESS) + "=" + _sliderBr.value;
	ret += "&" + (offset + URL_INDEX_OF_R) + "=" + _sliderR.value;
	ret += "&" + (offset + URL_INDEX_OF_G) + "=" + _sliderG.value;
	ret += "&" + (offset + URL_INDEX_OF_B) + "=" + _sliderB.value;
	
	
	return ret;
}

function getSelectedButton(name) { 
	var buttons = document.getElementsByName(name); 
	
	for(i = 0; i < buttons.length; i++) { 
		if(buttons[i].checked) 
			return i;
	}
}

function rangeBrightness(id){ 
	var range = document.getElementById(id); 
	var value = parseInt(range.value, 10).toString(16);
	if( value.length == 1 )
		value = "0" + value;
	range.style.background = "#" + value + value + value;
	
}