import tables,io,os
import logging
from sys import argv
import time
import numpy as np
from swiftclient.multithreading import OutputManager
from swiftclient.service import SwiftError, SwiftService, SwiftUploadObject
logging.basicConfig(level=logging.ERROR)
logging.getLogger("requests").setLevel(logging.CRITICAL)
logging.getLogger("swiftclient").setLevel(logging.CRITICAL)
logger = logging.getLogger(__name__)
