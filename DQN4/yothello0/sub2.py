import subprocess


def aa(event, context):
    board = event["board"]
    #cmu = ["./yothello", "000000100800000000000008100000001"]
    cmu = ["./yothello", board]
    string = subprocess.run(cmu, stdout=subprocess.PIPE)
    stdout = string.stdout.decode("utf-8")
    return stdout
