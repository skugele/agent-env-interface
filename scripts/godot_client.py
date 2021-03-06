#
# A ZeroMQ Client (Request-Reply) for Submitting Agent Actions to Godot.
#

import zmq
import json
import argparse
import sys
import os

DEFAULT_TIMEOUT = 5000  # in milliseconds
DEFAULT_PORT = 5678


def parse_args():
    parser = argparse.ArgumentParser(description='A Network Client for Requesting Agent Actions in Godot')

    parser.add_argument('--request', type=str, required=True, help='the request to send')
    parser.add_argument('--count', type=int, required=False, default=1,
                        help='the number of action requests to send')
    parser.add_argument('--port', type=int, required=False, default=DEFAULT_PORT,
                        help='the port number of the Godot action listener')
    parser.add_argument('--verbose', required=False, action="store_true",
                        help='increases verbosity (displays requests & replies)')

    return parser.parse_args()


def establish_connection(args):
    context = zmq.Context()

    socket = context.socket(zmq.REQ)
    socket.connect('tcp://localhost:' + str(args.port))

    # without a timeout on receive the process can hang indefinitely
    socket.setsockopt(zmq.RCVTIMEO, DEFAULT_TIMEOUT)

    return socket


def send_request(connection, request):
    # request_as_json = json.dumps(request)
    if args.verbose:
        # print("request: ", request_as_json)
        print("request: ", request)

    connection.send_string(request)


def receive_reply(connection, args):
    reply = connection.recv_json()
    if args.verbose:
        print("reply: ", reply)

    return reply


if __name__ == "__main__":
    try:
        # parse command line arguments
        args = parse_args()

        # establish connection to Godot action server
        connection = establish_connection(args)

        # create action request
        request = args.request

        # TODO: What happens if a json compliant string is send to the json marshaller?
        # request['id'] = args.id
        # request['action'] = args.action

        for request_id in range(args.count):
            send_request(connection, request)
            _ = receive_reply(connection, args)

    except KeyboardInterrupt:

        try:
            sys.exit(1)
        except SystemExit:
            os._exit(1)
