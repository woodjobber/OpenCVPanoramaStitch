//
//  SSViewController.swift
//  SwiftStitch
//
//  Created by chengbin on 2022/7/8.
//  Copyright © 2022 ellipsis.com. All rights reserved.
//

import UIKit

class SSViewController: UIViewController {

    @IBOutlet weak var panoramaView: CTPanoramaView!
   
    var image: UIImage?
    
    override func viewDidLoad() {
        super.viewDidLoad()
    
    }
    
    override func viewDidAppear(_ animated: Bool){
        super.viewDidAppear(animated);
        panoramaView.panoramaType = .spherical
        panoramaView.controlMethod = .touch
        loadSphericalImage()
    }


    func loadSphericalImage() {
        panoramaView.image = image
    }

    override var supportedInterfaceOrientations: UIInterfaceOrientationMask {
        return .all
    }

}
