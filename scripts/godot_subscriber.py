#
# A subscriber to Godot sensor messages, optionally filtered by topic.
#

import sys
import zmq
import json


def split(m):
    """ Splits message into separate topic and content strings.
    :param m: a ZeroMq message containing the topic string and JSON content
    :return: a tuple containing the topic and JSON content
    """
    ndx = m.find('{')
    return m[0:ndx - 1], m[ndx:]


#  creates a subscriber socket
context = zmq.Context()
socket = context.socket(zmq.SUB)

print("Collecting updates from Godot...")
socket.connect("tcp://localhost:9001")

# subscribe to
topic_filter = sys.argv[1] if len(sys.argv) > 1 else ''

# Python 2 - ascii bytes to unicode str
if isinstance(filter, bytes):
    topic_filter = filter.decode('ascii')

socket.setsockopt_string(zmq.SUBSCRIBE, topic_filter)

while True:
    msg = socket.recv_string()
    _, content = split(msg)

    print(content)

    # unmarshal JSON message content
    ld_string = json.loads(content)
    # print(ld_string['header']['seqno'])
