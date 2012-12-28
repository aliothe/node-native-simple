{
    'targets':[
        {
            'target_name': 'native_module',
            'sources': [
                'src/native_module.cc',
                'src/native.cc'
            ],
	    'conditions': [
                ['OS=="linux"', {
                    'cflags_cc': [ '-std=c++0x' ]
                }]
        }
    ]
}
