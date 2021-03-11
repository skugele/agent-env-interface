#
# An action requester
#

import zmq
import json

context = zmq.Context()

#  Socket to talk to server
print("Connecting to hello world server...")
socket = context.socket(zmq.REQ)
socket.connect("tcp://localhost:5678")

#  Do 10 requests, waiting each time for a response
for n in range(1000000):
    # print("Sending request %s ..." % n)

    request = {'agent':[1,2,3,4],
               'action':'my action',
               "int":1,
               "float":1.01,
               "boolean":False,
               "null":None,
               "dictionary":{'key1':10.123,
                             'key2':True,
                             'key3':{'inner_k':'inner_v'}}}
    socket.send_string(json.dumps(request))

    #  Get the reply.
    message = socket.recv_json()
    # print("Received reply %s [ %s ]" % (request, message))