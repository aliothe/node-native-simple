
// We wrap the native module here so that
// functions from the native module can be passed around 
// as first class citizens without missing the context

function  wrapper(){
    this.addon = require("./build/Release/native_module");
    this.instance = this.addon();
    var that = this;
    return {
        fib: function(n, cb)
        {
            that.instance.fib(n, cb);
        },
        fibsync: function(n)
        {
            return that.instance.fibsync(n);
        }
    };
}

module.exports.create = function() { return new wrapper(); };







