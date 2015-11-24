
function absoluteValue(value){
    var negated = 0-value

    if(negated > value){
        return negated
    }
    document.write("This must not print")
    return value
}


function equal(a,b){
    var resultTrue = 1
    var resultFalse = 0
    var sub = a-b
    var x = absoluteValue(sub)
    document.write(x)
    document.write(" ")

    var epsilon = 0.00001
    if(x < epsilon){
        return resultTrue
    }
    return resultFalse
}


var number = 5
var zero = 0
var n=15 - 50
var x=12
var result = equal(n,x)
var midPoint = absoluteValue(n)
document.write(result)
document.write("<br>")
document.write(midPoint)