describe("node native basic", 
         function() 
         {                   
             var _ = require('underscore'),
	     async = require('async'),
             module_path = '../..',
	     expected_result = [0,1,1,2,3,5,8,13,21,34,55,89,144,233,377,610,987,
				1597,2584,4181,6765,10946,17711,28657,46368,75025,
				121393,196418,317811,514229,832040,1346269,
				2178309,3524578,5702887,9227465,14930352,24157817,
				39088169];	     

             it("fib async", 
                function(done) 
                {
		    var start = 0,
		    end = 80,
		    node_native = require(module_path).create();
                    expect(node_native).not.toBeNull();
		    async.map(_.range(start,end), function(n, cb) { node_native.fib(n, cb); }, 
			      function(err, results)
		    		  {
		    		      expect(err).toBeNull();
		    		      results.sort(function(a,b){return a-b});
		    		      expect(_.isEqual(expected_result, _.take(results, expected_result.length))).toBe(true);
		    		      console.log(JSON.stringify(results));
		    		      done();
		    		  });
                });          


	     it("fib sync", 
                function() 
                {
		    var start = 0,
		    end = 80,
		    node_native = require(module_path).create(),
		    results = [];
                    expect(node_native).not.toBeNull();
		    for(var i=start; i<=end; ++i)
			{
			    results.push(node_native.fibsync(i));
			}
		    console.log(JSON.stringify(results.sort(function(a,b){return a-b})));
                });          

         });
