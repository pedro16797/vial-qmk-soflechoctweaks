#!/usr/bin/env python3
import sys
import os
sys.path.append(os.path.join(os.getcwd(), "lib/python"))
os.environ["ORIG_CWD"] = os.getcwd()
from milc import cli
import qmk.cli
cli()
