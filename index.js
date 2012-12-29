
// We wrap the native module here so that
// functions from the native module can be passed around 
// as first class citizens without missing the context

function  wrapper(){
    this.native_module = require("./build/Release/native_module").create();
    var that = this;
    return {
	fib: function(n, cb)
	{
	    that.native_module.fib(n, cb);
	},
	fibsync: function(n)
	{
	    return that.native_module.fibsync(n);
	}
    };
};

module.exports.create = function() { return new wrapper(); };
