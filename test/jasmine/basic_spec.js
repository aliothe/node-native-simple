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
                    async.map(_.range(start,end), node_native.fib, 
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
                    results = _.map(_.range(start,end), node_native.fibsync);
                    expect(_.isEqual(expected_result, _.take(results, expected_result.length))).toBe(true);
                    console.log(JSON.stringify(results));
                });          

             it("fib async overflow", 
                function(done) 
                {
                    var start = 1470,
                    end = 1478,
                    node_native = require(module_path).create();
                    expect(node_native).not.toBeNull();
                    async.map(_.range(start,end), node_native.fib, 
                              function(err, results)
                                  {
				      expect(err).not.toBeNull();
				      if(null !== err)
					  {
					      expect(err instanceof Error).toBeTruthy();
					      expect(err.message).toBe('overflow, result does not fit in double');
					  }
                                      done();
                                  });
                });          

             it("fib sync overflow", 
                function() 
                {
                    var start = 1470,
                    end = 1478,
                    node_native = require(module_path).create(),
                    results = [];
                    expect(node_native).not.toBeNull();
		    expect(function(){results = _.map(_.range(start,end), node_native.fibsync);}).toThrow('overflow, result does not fit in double');
		    try{
			results = _.map(_.range(start,end), node_native.fibsync);
		    }catch(err){
			expect(err).not.toBeNull();
			if(null !== err)
			{
			    expect(err instanceof Error).toBeTruthy();
			    expect(err.message).toBe('overflow, result does not fit in double');
			}
		    }
                });          

         });
