const URL_INDEX_OF_EFFECT             	= 1;
const URL_INDEX_OF_SPEED              	= 2;
const URL_INDEX_OF_BRIGHTNESS         	= 3;

const URL_INDEX_OF_R                  	= 5;
const URL_INDEX_OF_G                  	= 6;
const URL_INDEX_OF_B                  	= 7;
const JS_INDEX_OF_BUTTON				= 9;
const URL_OFFSET_TO_NEXT_STRIP        	= 10;

//EXPAND
const ELEMENTS_NUMBER					= 2;

var elementsAll = [];
let i;
for(i = 0; i < ELEMENTS_NUMBER; i++){
	elementsAll[i] = [
		0,
		document.getElementsByName("radio" + i),
		document.getElementById("rangeSpeed" + i),
		document.getElementById("rangeBrightness" + i),
		0,
		document.getElementById("rangeR" + i),
		document.getElementById("rangeG" + i),
		document.getElementById("rangeB" + i),
		0,
		document.getElementById("button" + i)
	];
}

var currentURL = new URL(document.URL);

function onInput(_id){
	changeColor(_id.replace("sliderContainer",""));
}
function changeColor(_index){
	let _sliderR = elementsAll[_index][URL_INDEX_OF_R];
	let _sliderG = elementsAll[_index][URL_INDEX_OF_G];
	let _sliderB = elementsAll[_index][URL_INDEX_OF_B];
	let _buttons = elementsAll[_index][JS_INDEX_OF_BUTTON];
	
	let r = parseInt(_sliderR.value, 10).toString(16);
	let g = parseInt(_sliderG.value, 10).toString(16);
	let b = parseInt(_sliderB.value, 10).toString(16);
	
	if(r.length === 1)
		r = "0" + r;
	if(g.length === 1)
		g = "0" + g;
	if(b.length === 1)
		b = "0" + b;
	
	let color = "#" + r + g + b;
	_buttons.style.background = color;
	_buttons.style.color = invertColor(color);
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
    let r = (255 - parseInt(hex.slice(0, 2), 16)).toString(16),
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
	let valuePairs = location.search.substr(1).split("&");
	let i;
	for(i = 0; i < valuePairs.length; i++) { 
		let valuePair = valuePairs[i].split("=");
		let elementToSet = elementsAll[Math.floor(valuePair[0] / URL_OFFSET_TO_NEXT_STRIP)]
		[valuePair[0] % URL_OFFSET_TO_NEXT_STRIP];
		if((valuePair[0] % URL_OFFSET_TO_NEXT_STRIP ) == URL_INDEX_OF_EFFECT){	// redio buttons
			elementToSet[valuePair[1]].checked = true;
		}
		else if(false){	// for the timer
		}
		else{
			elementToSet.value = parseInt(valuePair[1], 10);
		}
	}
	for(i = 0; i < elementsAll.length; i++) {
		changeColor(i);
		rangeBrightness("rangeBrightness" + i);
		rangeSpeed("rangeSpeed" + i);
	}
}

function buttonClick(_index){
	_index = _index.replace("button","");
	if(_index == 999){
		let redirectLink = "/command?";
		let i;
		for(i = 0; i < elementsAll.length; i++) { 
			redirectLink += getURLOf(i);
			if((i + 1) < elementsAll.length)
				redirectLink += "&";
		}
		window.location.href = redirectLink;
	}
	
	window.location.href = "/command?" + getURLOf(_index);
}

/* HELPER
const URL_INDEX_OF_EFFECT             = 1;
const URL_INDEX_OF_SPEED              = 2;
const URL_INDEX_OF_BRIGHTNESS         = 3;

const URL_INDEX_OF_R                  = 5;
const URL_INDEX_OF_G                  = 6;
const URL_INDEX_OF_B                  = 7;
const URL_OFFSET_TO_NEXT_STRIP        = 10;
*/

function getURLOf(_index){
	let _buttons = elementsAll[_index][URL_INDEX_OF_EFFECT];
	let _sliderSp = elementsAll[_index][URL_INDEX_OF_SPEED];
	let _sliderBr = elementsAll[_index][URL_INDEX_OF_BRIGHTNESS];
	
	let _sliderR = elementsAll[_index][URL_INDEX_OF_R];
	let _sliderG = elementsAll[_index][URL_INDEX_OF_G];
	let _sliderB = elementsAll[_index][URL_INDEX_OF_B];
	
	let ret = "";
	ret += (_index * URL_OFFSET_TO_NEXT_STRIP + URL_INDEX_OF_EFFECT) + "=" + getSelectedButton(_buttons);
	ret += "&" + (_index * URL_OFFSET_TO_NEXT_STRIP + URL_INDEX_OF_SPEED) + "=" + _sliderSp.value;
	ret += "&" + (_index * URL_OFFSET_TO_NEXT_STRIP + URL_INDEX_OF_BRIGHTNESS) + "=" + _sliderBr.value;
	ret += "&" + (_index * URL_OFFSET_TO_NEXT_STRIP + URL_INDEX_OF_R) + "=" + _sliderR.value;
	ret += "&" + (_index * URL_OFFSET_TO_NEXT_STRIP + URL_INDEX_OF_G) + "=" + _sliderG.value;
	ret += "&" + (_index * URL_OFFSET_TO_NEXT_STRIP + URL_INDEX_OF_B) + "=" + _sliderB.value;
	
	return ret;
}

function getSelectedButton(_buttons) {
	let i;
	for(i = 0; i < _buttons.length; i++) { 
		if(_buttons[i].checked) 
			return i;
	}
}

function rangeBrightness(id){ 
	let range = document.getElementById(id); 
	let value = parseInt(range.value, 10).toString(16);
	if( value.length == 1 )
		value = "0" + value;
	range.style.background = "#" + value + value + value;
	
}

function rangeSpeed(id){ 
	let range = document.getElementById(id);
	id = id.replace("Speed","").replace("range","");
	id = "textOnSlider" + id;
	let textField = document.getElementById(id); 
	
	let value = parseInt(range.value, 10);
	textField.textContent = value;
}