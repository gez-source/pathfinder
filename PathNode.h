/* Copyright © Mr Gerallt G. Franke 2017 */

/*+ Author: Mr Gerallt G. Franke
 *  Description: 2D / 3D Pathfinder.
 *  Date: 18/06/2017
 * Abstract: 2D / 3D Pathfinder.
 * 
 *                                      '
 *    [ PATHFINDER ]             . ` ;  ::  
 *                              , : +:;,,@ 
 *                            '#:. '#` + @; 
 *            ;               :+:,.@#;'.; +, 
 *            `               '+;.'@'+@@ :@+ 
 *           @              '+;;'+@@'@@' +@'@   
 *           @              :'::##@+##':@#@@''  
 *         +:+              #::;;@@,;@@+@##;@@:
 *        @'##            ` +;###@#@''#.@@###@@+  
 *        '#@@            .`@+@#;+@';@@#+@'##@@@++ 
 *       #@@@@            ++@++;;@':+;+:;@+++#@@+#+'  
 *       #@@@@#          ##@++#++:'+';@#@#:'';+@#;#;@ 
 *      '@@ @@@,        #+#;@#@##;+.,@#@+@+;;''#@#;@+@ 
 *      @#@@@@@#      `;@#@##@+##;#@,#@#@#@;:;;:+@@:@;#, 
 *      #@@@@@@@#    :'#,:+#'#++@#+#+;@@@@@;''::;'@#+,#'+  
 *     ,#@@@@@@@@@   ;'+@@;'#@+@#@@@@#+'@@@@;@@##::@;@;,'+  
 *     @#@@@@@@@@@@  :`+@###+;@';@'+;.`'@@@@@@@#@#@:@,@;:++  
 *    #@@@@@@@@@@@@@ #@;@#@@+'@'@+#@@@+@@@@@#@'@@#@@;@;@';@' 
 *    @@@@@    @@@@@@@++++@+'@@@@@@@#@@;+@@@###+@@@@@'#;+;'#@  
 *   +@@@@@@@@@@@@@@@#+@@@+@+;+@@,+@`@:+@@@###@+` @@@@#';+#;@; 
 *   @@@@@@@@@@@@@@@@@@;#'##+###@@#+@@@@'@@##@@@@      @';;@'# 
 *   @#@@@@@@@#@@@@@#@+#@+@#@;@@ FRANKE @@@##@@@#@     ,@:;'@;+ 
 *  #@#@@@@@@@     +@@:#++#+@@@@@#@@@@@@@@@@@@@@@@     ;#@:;'@#+  
 *  @##@@@@@#       @@@@+#+##'@@@@@++@@#@@@@@@@@@###;   @#@;''#@, 
 *  @@@@@@@@        '@##'#@;#@@@@@@@@@@@@@@@   @@@@#+`  @@'@;'';@'  
 * ,@@ @@@,         @#+#@''',#@@@@@@@@@@@@@@@@@@@@@#@#' `@@'@'+''@++;  
 * @##@@@@          .@##+#+#;@@@@@@@@#@@@@@@#@@@#@@@@+@. @  ;''++'#'++
 * @@@@@#           `#@#'+++`,+@@@@@#@@  +. #@@#  @@@##@ @   ;''.`'#+#+ 
 * @@@@'             #@+@+:+`` @@@#@+`      @@@+    @@#++    ,@'+ @'++# 
 *'@@@@              #@##@#+# #@@@@         @@@'     @@@@'    '@++ '+##+ 
 *@@@                @@@;@#'+,#@@+          @@@.      #@@@     '+'#+;++@'
 *@@;                '@#@##'#'@ @           @@@        @@+      '''@'+++#:
 *@@;                 ###@@@,#.             +#@        .@#,     ,''#''+++' 
 *@@                  `'@@'#@#.   ,         ###`        @++      ##' .;++#  
 *@@                +  :@@#+@,@+  ,     '   @@#+        ;#@:      '+   ++## 
 *@                  '  ++@.''::; '@   ;    ##@@:        #@:       +;   '++ 
 *@                  `;   @;@#+' #@;  :     @##@+        @#'     +  +    '+  
 *                    +'; :@'':: +'  #+     +##@@:       @:+        +     +' 
 *                     @''#';@+++: ,.@      #@@@@+       ##'         .    :# 
 *                      `@+ `';;+@' ;,       @#@@#@       '+               +   
 *                       @@@ ',,+# @`        @@#@@#       @;               ,:  
 *                         #;+.'':,          @@#,@@;      @+                #  
 *                         :@#+.''`           @# @@@      @#                .  
 *                         '@#''#;             @ @@#      @+                 ` 
 *                          @#@@#;             @:`@#      @#     
 *                           @@@+              @: @#      #+   
 *                            ''               @; '#      '+        
 *                            ,,               @. ,@      .++
 *                            `                @`  @      .@        
 *                                             +   @       @     
 *                                                 .        ' */

#ifndef PATH_NODE_H
#define PATH_NODE_H

#include <string>

#include "Vector3.h"

class PathNode
{
public:
    std::string Index; // for testing
    int x, y, z; // grid space

    Vector3 Position; // transform.position

    float DistFromStart;
    float DistToGoal;
    float NodeCollisionRadius;


    //PathManager Manager;
    int OwnerID;

    bool IsGoal;//test
    bool Debug;
    bool IsSolid = false;

    PathNode() 
    {

    }

    //bool HasCollisions()
    //{
    //    // Get all nearby collisions and check if they are not the owning character

    //    int collisions = Physics.OverlapSphere(Position, NodeCollisionRadius)
    //        .Where(c => c.gameObject. GetInstanceID() != OwnerID)
    //        .Count();
    //    return collisions > 0;
    //    //return Physics.CheckSphere(Position, NodeCollisionRadius);
    //}

    //bool DistToGround(float& distanceToGround)
    //{
    //    bool onGround;
    //    RaycastHit hitInfo;
    //    onGround = Physics.Linecast(Position, Position + new Vector3(0, Manager.Collider.bounds.min.y, 0), out hitInfo);
    //    distanceToGround = 0;
    //    if (onGround)
    //        distanceToGround = Vector3.Distance(hitInfo.point, transform.position);
    //    return onGround;
    //}

    //bool IsInAir()
    //{//CHECK:TEST: Check that when PathNode is in air works properly
    //    return !Physics.Linecast(transform.position, new Vector3(0, Manager.Collider.bounds.min.y, 0));
    //}

    //float RiskIndicator()
    //{
    //    float risk = 0;

    //    Collider[] coliders = Physics.OverlapSphere(Position, 4);

    //    foreach (Collider collider in coliders)
    //    {
    //        Dangerous danger = collider.gameObject.GetComponent<Dangerous>();
    //        if (danger != null)
    //        {
    //            switch (danger.Type) //TODO: parameterise risk level weights
    //            {
    //                case Dangerous.RiskType.HarmfulArea:
    //                    risk += 10;
    //                    break;
    //                case Dangerous.RiskType.Contact:
    //                    risk += 6;
    //                    break;
    //                case Dangerous.RiskType.LineOfSight:
    //                    risk += 1;
    //                    break;
    //                case Dangerous.RiskType.GravitationalBodyInvSqr:
    //                    risk += 1;
    //                    break;
    //                case Dangerous.RiskType.IncomingVirus:
    //                    risk += 1;
    //                    break;
    //                case Dangerous.RiskType.IncomingBigEater:
    //                    risk += 1;
    //                    break;
    //                case Dangerous.RiskType.IncomingLittleEater:
    //                    risk += 1;
    //                    break;
    //                case Dangerous.RiskType.IncomingEater:
    //                    risk += 1;
    //                    break;
    //            }
    //            switch (danger.Level)
    //            {
    //                case Dangerous.SeverityLevel.Benign:
    //                    risk += 1;
    //                    break;
    //                case Dangerous.SeverityLevel.Low:
    //                    risk += 1;
    //                    break;
    //                case Dangerous.SeverityLevel.Medium:
    //                    risk += 5;
    //                    break;
    //                case Dangerous.SeverityLevel.High:
    //                    risk += 7;
    //                    break;
    //                case Dangerous.SeverityLevel.CertainDeath:
    //                    risk += 20;
    //                    break;
    //            }
    //        }
    //    }

    //    return risk;
    //}
        

    std::string ToString()
    {
        return Index + " " + std::to_string(Position.x) + " " + std::to_string(Position.y) + std::to_string(Position.z);
    }
    
};

#endif