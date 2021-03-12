#
# A ZeroMQ Subscriber(Pub-Sub) for Receiving Sensor Messages from Godot
#

import argparse
import sys
import os
import json

import zmq  # ZeroMq

DEFAULT_TIMEOUT = 5000  # in milliseconds
DEFAULT_PORT = 9001


def parse_args():
    """ Parses command line arguments. """
    parser = argparse.ArgumentParser(description='A Network Client for Requesting Agent Actions in Godot')

    parser.add_argument('--topic', metavar='TOPIC', required=False,
                        help='the topics to subscribe', default='')
    parser.add_argument('--count', type=int, required=False, default=1,
                        help='the number of action requests to send')
    parser.add_argument('--port', type=int, required=False, default=DEFAULT_PORT,
                        help='the port number of the Godot action listener')
    parser.add_argument('--verbose', required=False, action="store_true",
                        help='increases verbosity (displays requests & replies)')

    return parser.parse_args()


def split(m):
    """ Splits message into separate topic and content strings.
    :param m: a ZeroMq message containing the topic string and JSON content
    :return: a tuple containing the topic and JSON content
    """
    ndx = m.find('{')
    return m[0:ndx - 1], m[ndx:]


def establish_connection(args):
    #  creates a subscriber socket
    context = zmq.Context()
    socket = context.socket(zmq.SUB)

    # filters messages by topic
    if args.verbose:
        print('subscribing to topic: ', str(args.topic))

    socket.setsockopt_string(zmq.SUBSCRIBE, str(args.topic))
    socket.setsockopt(zmq.RCVTIMEO, DEFAULT_TIMEOUT)

    socket.connect('tcp://localhost:' + str(args.port))

    return socket


if __name__ == "__main__":
    # parse command line arguments
    args = parse_args()

    try:

        # establish connection to Godot state publisher
        connection = establish_connection(args)

        while True:
            msg = connection.recv_string()

            topic, content = split(msg)

            # unmarshal JSON message content
            obj = json.loads(content)

            if args.verbose:
                print('new message on topic: ', topic)
                print('header: ', obj['header'])
                print('data: ', obj['data'])
                print('')
            else:
                print(content)

    except KeyboardInterrupt:

        try:
            sys.exit(1)
        except SystemExit:
            os._exit(1)
