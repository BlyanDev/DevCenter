//
//  NativeView.swift
//  GuiLiteDemo
//
//  Created by Zorro Young on 21/12/17.
//

import Cocoa

class NativeView: NSView {

    @IBOutlet var m_view: NSView!
    override func draw(_ dirtyRect: NSRect) {
        super.draw(dirtyRect)

        // Drawing code here.
        
        //let path = NSBezierPath()
        //path.move(to: CGPoint(x: 0 , y: 0))
        //path.line(to: CGPoint(x: 100 , y: 100))
        //path.stroke()
        
        //NSColor.red.setFill()
        //NSRectFill(dirtyRect)
        
        let img = NSImage(named: "flip.png")
        let bg = NSColor.init(patternImage: img!)
        bg.setFill()
        NSRectFill(dirtyRect)
    }
}
