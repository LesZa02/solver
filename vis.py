from tkinter import Tk, Canvas, Frame, BOTH
import time
import re
import sys


class Solver(Frame):
    def __init__(self, parent,  arr, x_cnt, y_cnt):
        Frame.__init__(self, parent, background="white")
        self.scale = 1
        self.y_cnt = y_cnt
        self.x_cnt = x_cnt
        self.canvas = None
        self.curStep = None
        self.path = None
        self.x = 1500
        self.y = 900
        self.x_sp = self.x / (x_cnt + 1)
        self.y_sp = self.y / (y_cnt + 1)
        self.parent = parent
        self.pack(fill=BOTH, expand=1)
        self.centerWindow()
        self.autoMode = False
        self.pointCoord = arr
        self.pointsId = dict()
        self.autoModeSleepTime = 0.1
        self.pointRadius = min(6, min(self.x_sp, self.y_sp) / 3)
        self.bind_all("<Key>", self.onKeyPressed)
        self.bind_all("<Configure>", self.resize)
        self.bind_all("<MouseWheel>", self.zoom)

    def centerWindow(self):
        sw = self.parent.winfo_screenwidth()
        sh = self.parent.winfo_screenheight()
        x = (sw - self.x) / 2
        y = (sh - self.y) / 2
        self.parent.geometry('%dx%d+%d+%d' % (self.x, self.y, x, y))

    def zoom(self, event):
        if event.num == 4 or event.delta == 120:
            self.scale *= 2
        elif event.num == 5 or event.delta == -120:
            self.scale *= 0.5
        self.redrawAllPoint(event.x, event.y)
        
    def onKeyPressed(self, e):
        key = e.keysym
        if key == "Left" and not self.autoMode:
            self.deleteBlock()
        if key == "Right" and not self.autoMode:
            self.printBlock()
        if key == "Up" and self.autoMode:
            self.autoModeSleepTime /= 2
        if key == "Down" and self.autoMode:
            self.autoModeSleepTime *= 2
        if key == "space":
            if not self.autoMode:
                self.autoPrint()
            else:
                self.autoMode = False

    def resize(self, event):
        if event.width != self.x or event.height != self.y:
            self.x = event.width
            self.y = event.height
            self.x_sp = self.x / (self.x_cnt + 1)
            self.y_sp = self.y / (self.y_cnt + 1)
            self.config(width=self.x, height=self.y)
            self.redrawAllPoint()

    def redrawAllPoint(self, x_coord=0, y_coord=0):
        radius = self.pointRadius * self.scale
        for i in range(0, len(self.pointCoord), 1):
            coord_tuple = (self.pointCoord[i][0], self.pointCoord[i][1])
            new_x = self.scale * (self.x_sp + self.x_sp * self.pointCoord[i][0] - x_coord) + x_coord
            new_y = self.scale * (self.y_sp + self.y_sp * self.pointCoord[i][1] - y_coord) + y_coord
            self.canvas.coords(self.pointsId[coord_tuple], new_x - radius, new_y - radius,
                               new_x + radius, new_y + radius)
        self.canvas.pack(fill=BOTH, expand=1)

    def paintPoint(self, arr, color):
        for i in range(0, len(arr), 1):
            coord_tuple = (arr[i][0], arr[i][1])
            self.canvas.itemconfig(self.pointsId[coord_tuple], outline=color, fill=color, width=0)
        self.canvas.pack(fill=BOTH, expand=1)

    def printAllPoint(self):
        self.pack(fill=BOTH, expand=1)
        self.canvas = Canvas(self)
        for i in range(0, len(self.pointCoord), 1):
            coord_tuple = (self.pointCoord[i][0], self.pointCoord[i][1])
            new_x = self.x_sp + self.x_sp * self.pointCoord[i][0]
            new_y = self.y_sp + self.y_sp * self.pointCoord[i][1]
            self.pointsId[coord_tuple] = self.canvas.create_oval(new_x - self.pointRadius, new_y - self.pointRadius,
                                                                 new_x + self.pointRadius, new_y + self.pointRadius,
                                                                 outline="black", fill="black", width=0)
        self.canvas.pack(fill=BOTH, expand=1)

    def printBlock(self):
        if self.curStep + 1 < len(self.path):
            self.paintPoint(self.path[self.curStep], "green2")
            self.curStep += 1
            self.paintPoint(self.path[self.curStep], "red")

    def deleteBlock(self):
        if self.curStep - 1 >= 0:
            self.paintPoint(self.path[self.curStep], "black")
            self.curStep -= 1
            self.paintPoint(self.path[self.curStep], "red")

    def autoPrint(self):
        start_step = self.curStep
        self.autoMode = True
        for i in range(start_step, len(self.path), 1):
            if self.autoMode:
                self.printBlock()
                self.parent.update_idletasks()
                self.parent.update()
                time.sleep(self.autoModeSleepTime)
        self.autoMode = False

    def start(self):
        self.curStep = 0
        self.paintPoint(self.path[0], "red")

    def printPath(self, arr):
        self.path = arr
        self.start()


def readData(point, path):
    file = open('./vis.txt')
    data = file.readlines()
    point_cnt = int(re.search(r'\d+', data[0]).group())
    x_min = sys.maxsize
    y_min = sys.maxsize
    x_max = -sys.maxsize
    y_max = -sys.maxsize
    for i in range(1, point_cnt + 1, 1):
        x, y = re.findall(r'\d+', data[i])
        point.append([int(x), int(y)])
        x_min = int(x) if int(x) < x_min else x_min
        x_max = int(x) if int(x) > x_max else x_max
        y_min = int(y) if int(y) < y_min else y_min
        y_max = int(y) if int(y) > y_max else y_max

    cur_line = point_cnt + 4
    while cur_line < len(data) and (data[cur_line] != "\n"):
        clyaksa_size = int(re.search(r'\d+', data[cur_line]).group())
        clyaksa = []
        for i in range(0, clyaksa_size, 1):
            x, y = re.findall(r'\d+', data[cur_line + 1 + i])
            clyaksa.append([int(x), int(y)])
        path.append(clyaksa)
        cur_line += clyaksa_size + 2
    return x_max - x_min + 1, y_max - y_min + 1


def main():
    point = []
    path = []
    size = readData(point, path)
    print(size[0], size[1])

    root = Tk()
    sol = Solver(root, point, size[0], size[1])
    sol.printAllPoint()
    sol.printPath(path)

    root.mainloop()


if __name__ == '__main__':
    main()
